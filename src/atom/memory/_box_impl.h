#pragma once
// #include "atom/core.h"
// #include "atom/core/static_storage.h"
#include "atom/exceptions.h"

// #include "atom/invokable/invokable_ptr.h"

namespace atom
{
    template <typename in_value_type, bool in_copy, bool in_move, bool in_allow_non_move,
        usize in_buf_size, typename in_allocator_type>
    class _box_impl
    {
    public:
        using value_type = in_value_type;
        using allocator_type = in_allocator_type;

        class copy_tag
        {};

        class move_tag
        {};

    public:
        constexpr _box_impl()
            : _val()
            , _heap_mem(nullptr)
            , _heap_mem_size(0)
            , _alloc()
        {}

        template <typename box_type>
        constexpr _box_impl(copy_tag, const box_type& box)
        {}

        template <typename box_type>
        constexpr _box_impl(move_tag, box_type& box)
        {}

        template <typename value_type>
        constexpr _box_impl(value_type&& val)
        {}

        constexpr ~_box_impl() {}

    public:
        static consteval auto is_copyable() -> bool
        {
            return in_copy;
        }

        static consteval auto is_movable() -> bool
        {
            return in_move;
        }

        static consteval auto allow_non_movable() -> bool
        {
            return in_allow_non_move;
        }

        static consteval auto buf_size() -> usize
        {
            return in_buf_size;
        }

        /// ----------------------------------------------------------------------------------------
        /// copies `that` [`box`] into `this` [`box`].
        /// ----------------------------------------------------------------------------------------
        template <typename box_type>
        constexpr auto copy_box(box_type& that)
        {
            _copy_val(that);
        }

        /// ----------------------------------------------------------------------------------------
        /// moves `that` [`box`] into `this` [`box`].
        /// ----------------------------------------------------------------------------------------
        template <typename box_type>
        constexpr auto move_box(box_type&& that)
        {
            // when allocator type is different, we cannot handle heap memory. so we only move the
            // object and not the memory.
            if constexpr (not rsame_as<allocator_type,
                              typename tti::remove_cvref_type<box_type>::allocator_type>)
            {
                if (that._has_val())
                {
                    if (_has_val())
                    {
                        _destroy_val();
                    }

                    _move_val(that);
                }
                else
                {
                    if (_has_val())
                    {
                        _destroy_val();
                        _reset_val_data();
                    }
                }

                return;
            }

            if (that._has_val())
            {
                if (_has_val())
                {
                    _destroy_val();
                }

                if (that._is_val_on_buf())
                {
                    // if that has enough memory, we prefer that's memory as
                    // we are moving and user expects the memory to be moved too.
                    if (that._heap_mem_size >= that._val.size)
                    {
                        _check_and_release_mem();

                        _alloc = mov(that._alloc);
                        _heap_mem = that._heap_mem;
                        _heap_mem_size = that._heap_mem_size;

                        that._heap_mem = 0;
                        that._heap_mem_size = 0;
                    }

                    if (_heap_mem_size >= that._val.size)
                    {
                        _move_val_data(that);
                        _val.move(_heap_mem, _val.val);
                        _val.val = _heap_mem;
                    }
                    else
                    {
                        _move_val(that);
                    }
                }
                else
                {
                    _check_and_release_mem();

                    _alloc = mov(that._alloc);
                    _heap_mem = that._heap_mem;
                    _heap_mem_size = that._heap_mem_size;

                    that._heap_mem = 0;
                    that._heap_mem_size = 0;

                    _move_val_data(that);
                }
            }
            else
            {
                if (_has_val())
                {
                    _destroy_val();
                    _reset_val_data();
                }
            }
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type, typename... arg_types>
        constexpr auto emplace_val(arg_types&&... args, bool force_heap = false)
        {
            destroy_val();
            _emplace_val<type>(forward<arg_types>(args)..., force_heap);
        }

        /// ----------------------------------------------------------------------------------------
        /// destroys previous object if any and stores new object.
        ///
        /// @tparam type type of object to store.
        ///
        /// @param[in] val valect to store.
        /// @param[in] force_heap (default = false) force store on heap.
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto set_val(type&& val, bool force_heap = false)
        {
            emplace_val<tti::remove_cvref_type<type>>(forward<type>(val), force_heap);
        }

        /// ----------------------------------------------------------------------------------------
        /// get pointer to stored object.
        ///
        /// # template parameters
        ///
        /// - `type`: type as which to get the object.
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto get_val() -> const type&
        {
            debug_expects(get_mem_as<type>() != nullptr);

            return *get_mem_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        /// get pointer to stored object.
        ///
        /// # template parameters
        ///
        /// - `type`: type as which to get the object.
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto get_mut_val() -> type&
        {
            debug_expects(get_mut_mem_as<type>() != nullptr);

            return *get_mut_mem_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        /// get pointer to stored object.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_mem() const -> const void*
        {
            return _val.val;
        }

        /// ----------------------------------------------------------------------------------------
        /// get pointer to stored object.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_mut_mem() -> void*
        {
            return _val.val;
        }

        /// ----------------------------------------------------------------------------------------
        /// get pointer to stored object.
        ///
        /// @tparam type type as which to get the object.
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto get_mem_as() const -> const type*
        {
            return static_cast<type*>(_val.val);
        }

        /// ----------------------------------------------------------------------------------------
        /// get pointer to stored object.
        ///
        /// @tparam type type as which to get the object.
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto get_mut_mem_as() -> type*
        {
            return static_cast<type*>(_val.val);
        }

        /// ----------------------------------------------------------------------------------------
        /// get `std::type_info` of stored object.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_val_type() const -> const std::type_info&
        {
            return *_val.type;
        }

        /// ----------------------------------------------------------------------------------------
        /// size of the object stored.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_val_size() const -> usize
        {
            return _val.size;
        }

        /// ----------------------------------------------------------------------------------------
        /// checks if object is not `null`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto has_val() const -> bool
        {
            return _has_val();
        }

        /// ----------------------------------------------------------------------------------------
        /// disposes current object by calling its destructor.
        /// ----------------------------------------------------------------------------------------
        constexpr auto destroy_val()
        {
            if (_has_val())
            {
                _destroy_val();
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// is object is stored in stack memory.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_val_on_buf() const -> bool
        {
            return _is_val_on_buf();
        }

    private:
        /// ----------------------------------------------------------------------------------------
        /// creates new object of type `type`. does not handles current object.
        ///
        /// # template parameters
        ///
        /// - `type`: type of the object to create.
        /// - `arg_types`: type of args for object's constructor.
        ///
        /// # parameters
        ///
        /// - `args...`: args for object's constructor.
        /// - `force_heap`: if true, allocates object on heap even if buffer was sufficient, else
        ///    chooses the best fit.
        /// ----------------------------------------------------------------------------------------
        template <typename type, typename... arg_types>
        constexpr auto _emplace_val(arg_types&&... args, bool force_heap = false)
        {
            _val.size = sizeof(type);
            _val.type = &typeid(type);

            // todo: check if we can do static_cast instead to preserve constexpr.
            _val.dtor = [](void* val) { reinterpret_cast<type*>(val)->type::~type(); };

            if constexpr (is_copyable())
            {
                _val.copy = [](void* val, const void* that) {
                    new (val) type(*reinterpret_cast<const type*>(that));
                };
            }

            if constexpr (is_movable())
            {
                if constexpr (rmove_constructible<type>)
                {
                    _val.move = [](void* val, void* that) {
                        new (val) type(mov(*reinterpret_cast<type*>(that)));
                    };
                }
                else
                {
                    _val.move = nullptr;
                }
            }

            // if the object is not movable but in_allow_non_move is allowed, we allocate it on heap to
            // avoid object's move constructor.
            if constexpr (is_movable() and allow_non_movable() and not rmove_constructible<type>)
            {
                force_heap = true;
            }

            _val.val = _alloc_mem(_val.size, force_heap);
            new (_val.val) type(forward<arg_types>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// copies the object from `that` [`box`] into `this` [`box`].
        ///
        /// @param[in] that [`box`] of which to copy object.
        /// @param[in] force_heap (default = false) force allocate object on heap.
        /// ----------------------------------------------------------------------------------------
        template <typename box_type>
        constexpr auto _copy_val(const box_type& that, bool force_heap = false)
        {
            _copy_val_data(that);

            if constexpr (is_movable() and allow_non_movable())
            {
                force_heap = force_heap || _val.move == nullptr;
            }

            _val.val = _alloc_mem(_val.size, force_heap);
            _val.copy(_val.val, that.val);
        }

        /// ----------------------------------------------------------------------------------------
        /// moves the object from `that` [`box`] into `this` [`box`].
        ///
        /// @param[in] that [`box`] of which to move object.
        /// @param[in] force_heap (default = false) force allocate object on heap.
        ///
        /// @note this does not moves the memory from `that` [`box`].
        /// ----------------------------------------------------------------------------------------
        template <typename box_type>
        constexpr auto _move_val(box_type&& that, bool force_heap = false)
        {
            _move_val_data(that);

            if constexpr (allow_non_movable())
            {
                force_heap = force_heap || _val.move == nullptr;
            }

            _val.val = _alloc_mem(_val.size, force_heap);
            _val.move(_val.val, that._val.val);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto _has_val() const -> bool
        {
            return _val.val != nullptr;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto _destroy_val()
        {
            _val.dtor(_val.val);
            _reset_val_data();
        }

        /// ----------------------------------------------------------------------------------------
        /// copies {_val_data} from {that_box}.
        ///
        /// @param[in] that_box [`box`] of which to copy {_val_data}.
        /// ----------------------------------------------------------------------------------------
        template <typename box_type>
        constexpr auto _copy_val_data(const box_type& that_box)
        {
            const auto& that = that_box._val;

            _val.val = that.val;
            _val.size = that.size;
            _val.type = that.type;
            _val.dtor = that.dtor;

            if constexpr (is_copyable())
            {
                if constexpr (is_movable())
                {
                    _val.copy = that.copy;
                }
                else
                {
                    _val.copy = nullptr;
                }
            }

            if constexpr (is_movable())
            {
                if constexpr (box_type::is_movable())
                {
                    _val.move = that.move;
                }
                else
                {
                    _val.move = nullptr;
                }
            }
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename box_type>
        constexpr auto _move_val_data(box_type& that_box)
        {
            auto& that = that_box._val;

            _val.val = that.val;
            _val.type = that.type;
            _val.dtor = that.dtor;
            _val.size = that.size;

            that.val = nullptr;
            that.type = nullptr;
            that.dtor = nullptr;
            that.size = 0;

            if constexpr (is_copyable())
            {
                if constexpr (box_type::is_copyable())
                {
                    _val.copy = that.copy;
                    that.copy = nullptr;
                }
                else
                {
                    _val.copy = nullptr;
                }
            }

            if constexpr (is_movable())
            {
                if constexpr (box_type::is_movable())
                {
                    _val.move = that.move;
                    that.move = nullptr;
                }
                else
                {
                    _val.move = nullptr;
                }
            }
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto _reset_val_data()
        {
            _val = _val_data();
        }

        /// ----------------------------------------------------------------------------------------
        /// is object is stored in stack memory.
        /// ----------------------------------------------------------------------------------------
        constexpr auto _is_val_on_buf() const -> bool
        {
            if constexpr (buf_size() == 0)
                return false;

            return _val.val == _buf.mem().unwrap();
        }

        /// ----------------------------------------------------------------------------------------
        /// allocates enough memory of size `size`. uses stack memory if it is big enough.
        ///
        /// @param[in] size size of memory to allocate.
        /// @param[in] force_heap if `true`, allocates memory from `allocator_type`.
        ///
        /// @returns pointer to the allocated memory.
        /// ----------------------------------------------------------------------------------------
        constexpr auto _alloc_mem(usize size, bool force_heap = false) -> void*
        {
            if constexpr (buf_size() > 0)
            {
                // check if stack memory is big enough.
                if (not force_heap and size <= buf_size())
                {
                    return _buf.mut_mem().unwrap();
                }
            }

            // if we have previously allocated memory.
            if (_heap_mem != nullptr)
            {
                if (_heap_mem_size < size)
                {
                    _heap_mem = _alloc.realloc(_heap_mem, size).unwrap();
                    _heap_mem_size = size;
                }
            }
            // we need to allocate heap memory.
            else
            {
                _heap_mem = _alloc.alloc(size).unwrap();
                _heap_mem_size = size;
            }

            return _heap_mem;
        }

        /// ----------------------------------------------------------------------------------------
        /// if heap memory is allocated, deallocates it.
        /// ----------------------------------------------------------------------------------------
        constexpr auto _check_and_release_mem()
        {
            if (_heap_mem != nullptr)
            {
                _release_mem();
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// deallocates allocated heap memory.
        ///
        /// # note
        ///
        /// does not check if memory is even allocated.
        /// ----------------------------------------------------------------------------------------
        constexpr auto _release_mem()
        {
            _alloc.dealloc(_heap_mem);
            _heap_mem = nullptr;
            _heap_mem_size = 0;
        }

    private:
        class _val_data
        {
        public:
            value_type* val;
            usize size;
            const std::type_info* type;
            invokable_ptr<void(void*)> dtor;

            ATOM_CONDITIONAL_FIELD(is_copyable(), invokable_ptr<void(void*, const void*)>) copy;
            ATOM_CONDITIONAL_FIELD(is_movable(), invokable_ptr<void(void*, void*)>) move;
        };

    private:
        allocator_type _alloc;
        void* _heap_mem;
        usize _heap_mem_size;
        static_storage<buf_size()> _buf;
        _val_data _val;
    };
}
