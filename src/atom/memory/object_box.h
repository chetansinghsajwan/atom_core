#pragma once
#include "atom/core.h"
#include "atom/exceptions.h"
#include "atom/invokable/invokable_ptr.h"
#include "atom/tti.h"

#include "default_mem_allocator.h"

namespace atom
{
    namespace internal
    {
        /// ----------------------------------------------------------------------------------------
        /// `object_box_identifier` is used to check if the type is same as or derived from
        /// `object_box` template.
        /// ----------------------------------------------------------------------------------------
        class object_box_identifier
        {};
    }

    /// --------------------------------------------------------------------------------------------
    /// stores object inside using type erasure.
    ///
    /// # template parameters
    /// - `copyable`: should the `object_box` be `copy_constructible` and `copy_assignable`.
    /// - `movable`: should the `object_box` be `move_constructible` and `move_assignable`.
    /// - `allow_non_movable_object`: should the `object` be `move_constructible` when `movable` is
    ///     true. `object` can be allocated on heap if it's not movable.
    /// - `stack_size`: size of stack to store object, to avoid heap allocation.
    /// - `allocator_type`: `mem_allocator` to allocate memory to store object.
    /// --------------------------------------------------------------------------------------------
    template <bool copyable, bool movable, bool allow_non_movable_object, usize stack_size,
        typename allocator_type = default_mem_allocator>
    class object_box: public internal::object_box_identifier
    {
        template <bool other_copyable, bool other_movable, bool other_allow_non_movable_object,
            usize other_stack_size, typename tother_mem_allocator>
        friend class object_box;

        /// --------------------------------------------------------------------------------------------
        /// stores data for object. like, `copy_constructor`, `move_constructor` and `object_size`.
        /// --------------------------------------------------------------------------------------------
        class object_data
        {
        public:
            ATOM_CONDITIONAL_FIELD(copyable, invokable_ptr<void(mut_mem_ptr<void>, mem_ptr<void>)>)
            copy;

            ATOM_CONDITIONAL_FIELD(movable, invokable_ptr<void(mut_mem_ptr<void>, mut_mem_ptr<void>)>)
            move;

            invokable_ptr<void(mut_mem_ptr<void> obj)> dtor;

            usize size;
            mut_mem_ptr<void> obj;
            const type_info* type;
        };

        /// ----------------------------------------------------------------------------------------
        /// requirements for `object` accepted by this `object_box`.
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        static constexpr bool robject = requires {
            // `object_box` variants are not stored inside `object_box` variants.
            requires rnot_derived_from<type, internal::object_box_identifier>;

            // if box is `copyable` then the object should also be {copy_constructibl``.
            requires !copyable || rcopy_constructible<type>;

            // if box is `movable` then the object should also be `move_constructible` unless
            // `non_movable_object` is allowed.
            requires !movable || (rmove_constructible<type> || allow_non_movable_object);
        };

        /// ----------------------------------------------------------------------------------------
        /// requirements for other `object_box` accepted by this `object_box`.
        /// for example, `copy_constructor` and `move_constructor`.
        /// ----------------------------------------------------------------------------------------
        template <bool other_copyable, bool other_movable, bool other_allow_non_movable_object>
        static constexpr bool rother_box = requires {
            // if this {box} is `copyable` the {other_box} should also be `copyable`.
            requires !copyable || other_copyable;

            // if this {box} is `movable` the {other_box} should also be `movable` unless
            // `non_movable_object` is allowed.
            requires !movable || (other_movable || allow_non_movable_object);

            // if this {box} does not allow `non_movable_object` then {other_box} should also not
            // allow {non_movable_objec``.
            requires !movable || (allow_non_movable_object || !other_allow_non_movable_object);
        };

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr object_box()
            : _object()
            , _heap_mem(nullptr)
            , _heap_mem_size(0)
            , _allocator()
        {}

        /// ----------------------------------------------------------------------------------------
        /// # null constructor
        /// ----------------------------------------------------------------------------------------
        constexpr object_box(null_type null)
            : object_box()
        {}

        /// ----------------------------------------------------------------------------------------
        /// # null assignment
        /// ----------------------------------------------------------------------------------------
        auto operator=(null_type null) -> object_box&
        {
            _dispose_object();
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # constructor
        ///
        /// initializes with object.
        /// ----------------------------------------------------------------------------------------
        template <typename type>
            requires robject<type>
        object_box(type&& obj)
            : object_box()
        {
            _init_object(forward<type>(obj));
        }

        /// ----------------------------------------------------------------------------------------
        /// # assignment
        ///
        /// assigns new object.
        /// ----------------------------------------------------------------------------------------
        template <typename type>
            requires robject<type>
        auto operator=(type&& object) -> object_box&
        {
            _set_object(forward<type>(object));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # copy constructor
        /// ----------------------------------------------------------------------------------------
        object_box(const object_box& other)
            requires copyable
            : object_box()
        {
            _copy_box(other);
        }

        /// ----------------------------------------------------------------------------------------
        /// # copy constructor
        /// ----------------------------------------------------------------------------------------
        template <bool other_movable, bool other_allow_non_movable_object, usize other_stack_size,
            typename tother_mem_allocator>
            requires copyable && rother_box<copyable, other_movable, other_allow_non_movable_object>
        object_box(const object_box<copyable, other_movable, other_allow_non_movable_object,
            other_stack_size, tother_mem_allocator>& other)
            : object_box()
        {
            _copy_box(other);
        }

        /// ----------------------------------------------------------------------------------------
        /// # copy assignment
        /// ----------------------------------------------------------------------------------------
        auto operator=(const object_box& other) -> object_box&
            requires copyable
        {
            _copy_box(other);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # copy assignment
        /// ----------------------------------------------------------------------------------------
        template <bool other_movable, bool other_allow_non_movable_object, usize other_stack_size,
            typename tother_mem_allocator>
            requires copyable && rother_box<copyable, other_movable, other_allow_non_movable_object>
        auto operator=(const object_box<copyable, other_movable, other_allow_non_movable_object,
            other_stack_size, tother_mem_allocator>& other) -> object_box&
        {
            _copy_box(other);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # move constructor
        /// ----------------------------------------------------------------------------------------
        object_box(object_box&& other)
            requires movable
            : object_box()
        {
            _move_box(mov(other));
        }

        /// ----------------------------------------------------------------------------------------
        /// # move constructor
        /// ----------------------------------------------------------------------------------------
        template <bool other_copyable, bool other_movable, bool other_allow_non_movable_object,
            usize other_stack_size, typename tother_mem_allocator>
            requires movable && rother_box<other_copyable, other_movable, other_allow_non_movable_object>
        object_box(object_box<other_copyable, other_movable, other_allow_non_movable_object, other_stack_size,
            tother_mem_allocator>&& other)
            : object_box()
        {
            _move_box(mov(other));
        }

        /// ----------------------------------------------------------------------------------------
        /// # move assignment
        /// ----------------------------------------------------------------------------------------
        auto operator=(object_box&& other) -> object_box&
            requires movable
        {
            _move_box(mov(other));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # move assignment
        /// ----------------------------------------------------------------------------------------
        template <bool other_copyable, bool other_movable, bool other_allow_non_movable_object,
            usize other_stack_size, typename tother_mem_allocator>
            requires movable && rother_box<other_copyable, other_movable, other_allow_non_movable_object>
        auto operator=(object_box<other_copyable, other_movable, other_allow_non_movable_object,
            other_stack_size, tother_mem_allocator>&& other) -> object_box&
        {
            _move_box(mov(other));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # destructor
        /// ----------------------------------------------------------------------------------------
        ~object_box()
        {
            _dispose_box();
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// sets the new object.
        /// ----------------------------------------------------------------------------------------
        template <typename type>
            requires robject<type>
        auto set_object(type&& obj)
        {
            _set_object(forward<type>(obj));
        }

        /// ----------------------------------------------------------------------------------------
        /// get the object.
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        auto get_object() -> type&
        {
            return _get_object<type>().get_mut();
        }

        /// ----------------------------------------------------------------------------------------
        /// get the const object.
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        auto get_object() const -> const type&
        {
            return _get_object<type>().get();
        }

        /// ----------------------------------------------------------------------------------------
        /// # null equality operator
        /// ----------------------------------------------------------------------------------------
        auto eq(null_type null) const -> bool
        {
            return _has_object();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// box manipulation functions
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        /// ----------------------------------------------------------------------------------------
        /// copies `other` `object_box` into `this` `object_box`.
        /// ----------------------------------------------------------------------------------------
        template <bool other_movable, bool other_allow_non_movable_object, usize other_stack_size,
            typename tother_mem_allocator>
            requires copyable && rother_box<copyable, other_movable, other_allow_non_movable_object>
        auto _copy_box(const object_box<copyable, other_movable, other_allow_non_movable_object,
            other_stack_size, tother_mem_allocator>& other)
        {
            _copy_object(other);
        }

        /// ----------------------------------------------------------------------------------------
        /// moves `other` `object_box` into `this` `object_box`.
        /// ----------------------------------------------------------------------------------------
        template <bool other_copyable, bool other_movable, bool other_allow_non_movable_object,
            usize other_stack_size, typename tother_mem_allocator>
            requires movable && rother_box<other_copyable, other_movable, other_allow_non_movable_object>
        auto _move_box(object_box<other_copyable, other_movable, other_allow_non_movable_object,
            other_stack_size, tother_mem_allocator>&& other)
        {
            // when allocator type is different, we cannot handle heap memory.
            // so we only move the object.
            if constexpr (!rsame_as<allocator_type, tother_mem_allocator>)
            {
                _move_object(other);
                other._dispose_box();
                return;
            }

            _dispose_object();

            const usize other_obj_size = other._object.size;
            const bool other_is_using_stack_mem = other._is_using_stack_mem();
            if (other_is_using_stack_mem && other_obj_size > stack_size && _heap_mem_size >= other_obj_size
                && other._heap_mem_size < other_obj_size)
            {
                // we cannot deallocate our memory in the above scenario.
                other._release_mem();
            }
            else
            {
                _release_mem();

                _heap_mem = mov(other._heap_mem);
                _heap_mem_size = mov(other._heap_mem_size);
                _allocator = mov(other._allocator);
            }

            if (other_is_using_stack_mem)
            {
                _move_object(mov(other));
            }
            else
            {
                _copy_object_data(other);
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// destroy stored object and releases any allocated memory.
        /// ----------------------------------------------------------------------------------------
        auto _dispose_box()
        {
            _dispose_object();
            _release_mem();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// object manipulation functions
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        /// ----------------------------------------------------------------------------------------
        /// stores the object.
        ///
        /// @tparam type type of object to store.
        ///
        /// @param[in] obj object to store.
        /// @param[in] force_heap (default = false) force store on heap.
        ///
        /// @expects previous object is not set.
        /// ----------------------------------------------------------------------------------------
        template <typename type>
            requires robject<type>
        auto _init_object(type&& obj, bool force_heap = false)
        {
            _object.size = sizeof(type);
            _object.type = &typeid(type);

            _object.dtor = [](mut_mem_ptr<void> obj) { obj.template as<type>().get().type::~type(); };

            if constexpr (copyable)
            {
                _object.copy = [](mut_mem_ptr<void> obj, mem_ptr<void> other) {
                    new (obj.unwrap()) type(mem_ptr<type>(other).get());
                };
            }

            if constexpr (movable)
            {
                if constexpr (rmove_constructible<type>)
                {
                    _object.move = [](mut_mem_ptr<void> obj, mut_mem_ptr<void> other) {
                        new (obj.unwrap()) type(mov(mut_mem_ptr<type>(other).get_mut()));
                    };
                }
                else
                {
                    _object.move = nullptr;
                }
            }

            // if the object is not movable but allow_non_movable_object is allowed,
            // we allocate it on heap to avoid object's move constructor.
            if constexpr (movable && allow_non_movable_object && !rmove_constructible<type>)
            {
                force_heap = true;
            }

            _object.obj = _alloc_mem(_object.size, force_heap);
            new (_object.obj.unwrap()) type(forward<type>(obj));
        }

        /// ----------------------------------------------------------------------------------------
        /// destroys previous object if any and stores new object.
        ///
        /// @tparam type type of object to store.
        ///
        /// @param[in] obj object to store.
        /// @param[in] force_heap (default = false) force store on heap.
        /// ----------------------------------------------------------------------------------------
        template <typename type>
            requires robject<type>
        auto _set_object(type&& obj, bool force_heap = false)
        {
            _dispose_object();
            _init_object(forward<type>(obj));
        }

        /// ----------------------------------------------------------------------------------------
        /// get pointer to stored object.
        ///
        /// @tparam type type as which to get the object.
        /// ----------------------------------------------------------------------------------------
        template <typename type = void>
        auto _get_object() -> mut_mem_ptr<type>
        {
            return _object.obj;
        }

        /// ----------------------------------------------------------------------------------------
        /// get {type_info} or stored object.
        /// ----------------------------------------------------------------------------------------
        auto _get_object_type() const -> const type_info&
        {
            return *_object.type;
        }

        /// ----------------------------------------------------------------------------------------
        /// checks if object is not {null}.
        /// ----------------------------------------------------------------------------------------
        auto _has_object() const -> bool
        {
            return _object.obj != nullptr;
        }

        /// ----------------------------------------------------------------------------------------
        /// copies the object from `other` `object_box` into `this` `object_box`.
        ///
        /// @param[in] other `object_box` of which to copy object.
        /// @param[in] force_heap (default = false) force allocate object on heap.
        /// ----------------------------------------------------------------------------------------
        template <bool other_movable, bool other_allow_non_movable_object, usize other_stack_size,
            typename tother_mem_allocator>
            requires copyable && rother_box<copyable, other_movable, other_allow_non_movable_object>
        auto _copy_object(const object_box<copyable, other_movable, other_allow_non_movable_object,
                             other_stack_size, tother_mem_allocator>& other,
            bool force_heap = false)
        {
            _dispose_object();

            _copy_object_data(other);

            if constexpr (movable)
            {
                force_heap = force_heap || _object.move == nullptr;
            }
            else
            {
                force_heap = true;
            }

            _object.obj = _alloc_mem(_object.size, force_heap);
            _object.copy(_object.obj, other._object.obj);
        }

        /// ----------------------------------------------------------------------------------------
        /// moves the object from `other` `object_box` into `this` `object_box`.
        ///
        /// @param[in] other `object_box` of which to move object.
        /// @param[in] force_heap (default = false) force allocate object on heap.
        ///
        /// @note this_type doesn't moves the memory from `other` `object_box`.
        /// ----------------------------------------------------------------------------------------
        template <bool other_copyable, bool other_movable, bool other_allow_none_movable_object,
            usize other_stack_size, typename tother_mem_allocator>
            requires movable && rother_box<other_copyable, other_movable, other_allow_none_movable_object>
        auto _move_object(object_box<other_copyable, other_movable, other_allow_none_movable_object,
                             other_stack_size, tother_mem_allocator>&& other,
            bool force_heap = false)
        {
            _dispose_object();

            _copy_object_data(other);
            force_heap = force_heap || _object.move == nullptr;

            _object.obj = _alloc_mem(_object.size, force_heap);
            _object.move(_object.obj, other._object.obj);
        }

        /// ----------------------------------------------------------------------------------------
        /// disposes current object by calling its destructor.
        ///
        /// @note this_type does'n deallocates memory.
        ///
        /// @see _release_mem().
        /// ----------------------------------------------------------------------------------------
        auto _dispose_object()
        {
            if (_object.obj != nullptr)
            {
                _object.dtor(_object.obj);
                _object = {};
            }
        }

    private:
        using Base::_impl;
    };

    template <typename TVal, bool allowNonMove, usize bufSize, typename TAlloc>
    class MoveBox: public BoxFunctions<_BoxImpl<TVal, false, true, allowNonMove, bufSize, TAlloc>>
    {
        using This = MoveBox<TVal, allowNonMove, bufSize, TAlloc>;
        using Base = BoxFunctions<_BoxImpl<TVal, false, true, allowNonMove, bufSize, TAlloc>>;
        using _TImpl = typename Base::_TImpl;

    public:
        /// ----------------------------------------------------------------------------------------
        /// copies {object_data} from {other_box}.
        ///
        /// @param[in] other_box `object_box` of which to copy {object_data}.
        /// ----------------------------------------------------------------------------------------
        template <bool other_copyable, bool other_movable, bool other_allow_non_movable_object,
            usize other_stack_size, typename tother_mem_allocator>
        auto _copy_object_data(const object_box<other_copyable, other_movable,
            other_allow_non_movable_object, other_stack_size, tother_mem_allocator>& other_box)
        {
            auto& other = other_box._object;

            _object.obj = other.obj;
            _object.size = other.size;
            _object.type = other.type;
            _object.dtor = other.dtor;

            if constexpr (copyable)
            {
                if constexpr (movable)
                {
                    _object.copy = other.copy;
                }
                else
                {
                    _object.copy = nullptr;
                }
            }

            if constexpr (movable)
            {
                if constexpr (other_movable)
                {
                    _object.move = other.move;
                }
                else
                {
                    _object.move = nullptr;
                }
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// memory manipulation functions
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

    protected:
        /// ----------------------------------------------------------------------------------------
        /// allocates enough memory of size `size`. uses stack memory if it is big enough.
        ///
        /// @param[in] size size of memory to allocate.
        /// @param[in] force_heap if `true`, allocates memory from `allocator_type`.
        ///
        /// @returns pointer to the allocated memory.
        /// ----------------------------------------------------------------------------------------
        auto _alloc_mem(usize size, bool force_heap = false) -> mut_mem_ptr<void>
        {
            if constexpr (stack_size > 0)
            {
                // check if stack memory is big enough.
                if (!force_heap && size <= stack_size)
                {
                    return _stack_mem;
                }
            }

            // if we have previously allocated memory.
            if (_heap_mem != nullptr)
            {
                if (_heap_mem_size < size)
                {
                    _heap_mem = _allocator.realloc(_heap_mem, size);
                    _heap_mem_size = size;
                }
            }
            // we need to allocate heap memory.
            else
            {
                _heap_mem = _allocator.alloc(size);
                _heap_mem_size = size;
            }

            return _heap_mem;
        }

        /// ----------------------------------------------------------------------------------------
        /// deallocates any allocated memory.
        /// ----------------------------------------------------------------------------------------
        auto _release_mem()
        {
            if (_heap_mem != nullptr)
            {
                _allocator.dealloc(_heap_mem);
                _heap_mem = nullptr;
                _heap_mem_size = 0;
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// is object is stored in stack memory.
        /// ----------------------------------------------------------------------------------------
        auto _is_using_stack_mem() const -> bool
        {
            if constexpr (stack_size > 0)
            {
                return _object.obj.unwrap() == _stack_mem;
            }

        /// ----------------------------------------------------------------------------------------
        /// # Move Operator
        /// ----------------------------------------------------------------------------------------
        constexpr MoveBox& operator=(This&& that)
        {
            _impl.moveBox(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// stack memory.
        ///
        /// # to do
        /// - replace with a type to handle storage.
        /// ----------------------------------------------------------------------------------------
        ATOM_CONDITIONAL_FIELD(stack_size > 0, byte[stack_size.unwrap()]) _stack_mem;

        /// ----------------------------------------------------------------------------------------
        /// memory allocator.
        /// ----------------------------------------------------------------------------------------
        allocator_type _allocator;

        /// ----------------------------------------------------------------------------------------
        /// heap memory allocated.
        /// ----------------------------------------------------------------------------------------
        mut_mem_ptr<void> _heap_mem;

        /// ----------------------------------------------------------------------------------------
        /// size of heap memory allocated.
        /// ----------------------------------------------------------------------------------------
        usize _heap_mem_size;

        /// ----------------------------------------------------------------------------------------
        /// object data.
        /// ----------------------------------------------------------------------------------------
        object_data _object;
    };
}
