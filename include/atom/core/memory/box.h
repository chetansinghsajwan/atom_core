#pragma once
// #include "atom/core/_std.h"
// #include "atom/core/core.h"
// #include "atom/core/types.h"
// #include "atom/core/core/static_storage.h"
#include "atom/core/invokable/invokable_ptr.h"
// #include "atom/core/contracts.h"
// #include "atom/core/memory/default_mem_allocator.h"
#include "atom/core/preprocessors.h"

/// ------------------------------------------------------------------------------------------------
/// implementations
/// ------------------------------------------------------------------------------------------------
namespace atom
{
    template <typename in_value_t, bool in_copy, bool in_move, bool in_allow_non_move,
        usize in_buf_size, typename in_allocator_t>
    class _box_impl
    {
    public:
        using value_t = in_value_t;
        using allocator_t = in_allocator_t;

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

        template <typename box_t>
        constexpr _box_impl(copy_tag, const box_t& box)
        {
            copy_box(box);
        }

        template <typename box_t>
        constexpr _box_impl(move_tag, box_t& box)
        {
            move_box(box);
        }

        template <typename value_t>
        constexpr _box_impl(value_t&& val)
        {
            emplace_val<value_t>(forward<value_t>(val));
        }

        constexpr ~_box_impl()
        {
            destroy_val();
        }

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
        template <typename box_t>
        constexpr auto copy_box(box_t& that)
        {
            _copy_val(that);
        }

        /// ----------------------------------------------------------------------------------------
        /// moves `that` [`box`] into `this` [`box`].
        /// ----------------------------------------------------------------------------------------
        template <typename box_t>
        constexpr auto move_box(box_t&& that)
        {
            // when allocator type is different, we cannot handle heap memory. so we only move the
            // object and not the memory.
            if constexpr (not typeinfo<allocator_t>::template is_same_as<
                              typename typeinfo<box_t>::pure_t::value_t::allocator_t>)
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

                        _alloc = move(that._alloc);
                        _heap_mem = that._heap_mem;
                        _heap_mem_size = that._heap_mem_size;

                        that._heap_mem = 0;
                        that._heap_mem_size = 0;
                    }

                    if (_heap_mem_size >= that._val.size)
                    {
                        _move_val_data(that);
                        _val.move(_heap_mem, _val.val);
                        _val.val = reinterpret_cast<value_t*>(_heap_mem);
                    }
                    else
                    {
                        _move_val(that);
                    }
                }
                else
                {
                    _check_and_release_mem();

                    _alloc = move(that._alloc);
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
        template <typename type, typename... arg_ts>
        constexpr auto emplace_val(arg_ts&&... args)
        {
            destroy_val();
            _emplace_val<type, false>(forward<arg_ts>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type, typename... arg_ts>
        constexpr auto emplace_val_on_heap(arg_ts&&... args)
        {
            destroy_val();
            _emplace_val<type, true>(forward<arg_ts>(args)...);
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
            emplace_val<typename typeinfo<type>::pure_t::value_t>(forward<type>(val), force_heap);
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
            contract_debug_expects(get_mem_as<type>() != nullptr);

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
            contract_debug_expects(get_mut_mem_as<type>() != nullptr);

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
        constexpr auto get_val_t() const -> const std::type_info&
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
        /// - `arg_ts`: type of args for object's constructor.
        ///
        /// # parameters
        ///
        /// - `args...`: args for object's constructor.
        /// - `force_heap`: if true, allocates object on heap even if buffer was sufficient, else
        ///    chooses the best fit.
        /// ----------------------------------------------------------------------------------------
        template <typename type, bool force_heap, typename... arg_ts>
        constexpr auto _emplace_val(arg_ts&&... args)
        {
            _val.size = sizeof(type);
            _val.type = &typeid(type);

            // todo: check if we can do static_cast instead to preserve constexpr.
            _val.dtor = [](void* val) { reinterpret_cast<type*>(val)->type::~type(); };

            if constexpr (is_copyable())
            {
                _val.copy = [](void* val, const void* that)
                { new (val) type(*reinterpret_cast<const type*>(that)); };
            }

            if constexpr (is_movable())
            {
                if constexpr (typeinfo<type>::is_move_constructible)
                {
                    _val.move = [](void* val, void* that)
                    { new (val) type(move(*reinterpret_cast<type*>(that))); };
                }
                else
                {
                    _val.move = nullptr;
                }
            }

            bool on_heap = force_heap;

            // if the object is not movable but in_allow_non_move is allowed, we allocate it on heap to
            // avoid object's move constructor.
            if constexpr (is_movable() and allow_non_movable()
                          and not typeinfo<type>::is_move_constructible)
            {
                on_heap = true;
            }

            _val.val = _alloc_mem(_val.size, on_heap);
            new (_val.val) type(forward<arg_ts>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// copies the object from `that` [`box`] into `this` [`box`].
        ///
        /// @param[in] that [`box`] of which to copy object.
        /// @param[in] force_heap (default = false) force allocate object on heap.
        /// ----------------------------------------------------------------------------------------
        template <typename box_t>
        constexpr auto _copy_val(const box_t& that, bool force_heap = false)
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
        template <typename box_t>
        constexpr auto _move_val(box_t&& that, bool force_heap = false)
        {
            _move_val_data(that);

            if constexpr (allow_non_movable())
            {
                force_heap = force_heap || _val.move == nullptr;
            }

            _val.val = reinterpret_cast<value_t*>(_alloc_mem(_val.size, force_heap));
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
        template <typename box_t>
        constexpr auto _copy_val_data(const box_t& that_box)
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
                if constexpr (box_t::is_movable())
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
        template <typename box_t>
        constexpr auto _move_val_data(box_t& that_box)
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
                if constexpr (box_t::is_copyable())
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
                if constexpr (box_t::is_movable())
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

            return _val.val == _buf.mem();
        }

        /// ----------------------------------------------------------------------------------------
        /// allocates enough memory of size `size`. uses stack memory if it is big enough.
        ///
        /// @param[in] size size of memory to allocate.
        /// @param[in] force_heap if `true`, allocates memory from `allocator_t`.
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
                    return _buf.mut_mem();
                }
            }

            // if we have previously allocated memory.
            if (_heap_mem != nullptr)
            {
                if (_heap_mem_size < size)
                {
                    _heap_mem = _alloc.realloc(_heap_mem, size);
                    _heap_mem_size = size;
                }
            }
            // we need to allocate heap memory.
            else
            {
                _heap_mem = _alloc.alloc(size);
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
            value_t* val;
            usize size;
            const std::type_info* type;
            invokable_ptr<void(void*)> dtor;

            ATOM_CONDITIONAL_FIELD(is_copyable(), invokable_ptr<void(void*, const void*)>) copy;
            ATOM_CONDITIONAL_FIELD(is_movable(), invokable_ptr<void(void*, void*)>) move;
        };

    private:
        allocator_t _alloc;
        void* _heap_mem;
        usize _heap_mem_size;
        static_storage<buf_size()> _buf;
        _val_data _val;
    };
}

/// ------------------------------------------------------------------------------------------------
/// apis
/// ------------------------------------------------------------------------------------------------
namespace atom
{
    template <typename in_impl_t>
    class box_functions
    {
    protected:
        using _impl_t = in_impl_t;

    public:
        using value_t = typename _impl_t::value_t;

    public:
        template <typename... arg_ts>
        constexpr box_functions(arg_ts&&... args)
            : _impl(forward<arg_ts>(args)...)
        {}

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type, typename... arg_ts>
        constexpr auto emplace(arg_ts&&... args) -> type&
            requires(typeinfo<type>::template is_same_or_derived_from<value_t>)
        {
            _impl.template emplace_val<type>(forward<arg_ts>(args)...);
            return _impl.template get_mut_val_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto set(type&& obj) -> type&
            requires(typeinfo<type>::template is_same_or_derived_from<value_t>)
        {
            _impl._set_val(forward<type>(obj));
            return _impl.template get_mut_val_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto destroy()
        {
            _impl.destroy();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get() const -> const value_t&
        {
            contract_debug_expects(has_val(), "value is null.");

            return _impl.get_val();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_mut() -> value_t&
        {
            contract_debug_expects(has_val(), "value is null.");

            return _impl.template get_mut_val<value_t>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto check_get() const -> const value_t&
        {
            expects(has_val(), "value is null.");

            return _impl.get_val();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto check_get_mut() -> value_t&
        {
            expects(has_val(), "value is null.");

            return _impl.get_val_mut();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto get_as() const -> const type&
            requires(typeinfo<type>::template is_same_or_derived_from<value_t>)
        {
            contract_debug_expects(has_val(), "value is null.");

            return _impl.template get_val_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto get_mut_as() -> type&
            requires(typeinfo<type>::template is_same_or_derived_from<value_t>)
        {
            contract_debug_expects(has_val(), "value is null.");

            return _impl.template get_mut_val_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto check_get_as() const -> const type&
            requires(typeinfo<type>::template is_same_or_derived_from<value_t>)
        {
            expects(has_val(), "value is null.");

            return _impl.template get_val_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto check_get_mut_as() -> type&
            requires(typeinfo<type>::template is_same_or_derived_from<value_t>)
        {
            expects(has_val(), "value is null.");

            return _impl.template get_mut_val_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto mem() const -> const value_t*
        {
            return _impl.mem();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto mut_mem() -> const value_t*
        {
            return _impl.mut_mem();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto check_mem() const -> const value_t*
        {
            expects(has_val(), "value is null.");

            return _impl.mem();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto check_mut_mem() -> const value_t*
        {
            expects(has_val(), "value is null.");

            return _impl.mut_mem();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto mem_as() const
            -> const type* requires(typeinfo<type>::template is_same_or_derived_from<value_t>) {
            return _impl.template mem_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto mut_mem_as() -> const type* requires(
                                        typeinfo<type>::template is_same_or_derived_from<value_t>) {
            return _impl.template mut_mem_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto check_mem_as() const
            -> const type* requires(typeinfo<type>::template is_same_or_derived_from<value_t>) {
            expects(has_val(), "value is null.");

            return _impl.template mem_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto check_mut_mem_as()
            -> const type* requires(typeinfo<type>::template is_same_or_derived_from<value_t>) {
            expects(has_val(), "value is null.");

            return _impl.template mut_mem_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto val_t() const -> const std::type_info&
        {
            return _impl.get_val_t();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto val_size() const -> usize
        {
            return _impl.obj_size();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto has_val() const -> bool
        {
            return _impl.has_val();
        }

    protected:
        _impl_t _impl;
    };

    template <typename in_impl_t>
        requires typeinfo<typename in_impl_t::value_t>::is_void
    class box_functions<in_impl_t>
    {
    protected:
        using _impl_t = in_impl_t;

    public:
        using value_t = void;

    public:
        template <typename... arg_ts>
        constexpr box_functions(arg_ts&&... args)
            : _impl(forward<arg_ts>(args)...)
        {}

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type, typename... arg_ts>
        constexpr auto emplace(arg_ts&&... args) -> type&
            requires(not typeinfo<type>::is_void)
        {
            _impl.template emplace_val<type>(forward<arg_ts>(args)...);
            return _impl.template get_mut_val_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        /// requirements for `object` accepted by this `object_box`.
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto set(type&& obj) -> type&
        {
            _impl._set_val(forward<type>(obj));
            return _impl.template get_mut_val_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        /// requirements for other `object_box` accepted by this `object_box`.
        /// for example, `copy_constructor` and `move_constructor`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto destroy()
        {
            _impl.destroy_val();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto get_as() const -> const type&
        {
            contract_debug_expects(has_val(), "value is null.");

            return _impl.template get_val_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto get_mut_as() -> type&
        {
            contract_debug_expects(has_val(), "value is null.");

            return _impl.template get_mut_val_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto check_get_as() const -> const type&
        {
            expects(has_val(), "value is null.");

            return _impl.template get_val_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto check_get_mut_as() -> type&
        {
            expects(has_val(), "value is null.");

            return _impl.template get_mut_val_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto mem() const -> const void*
        {
            return _impl.get_mem();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto mut_mem() -> void*
        {
            return _impl.get_mut_mem();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto check_mem() const -> const void*
        {
            expects(has_val(), "value is null.");

            return _impl.get_mem();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto check_mut_mem() -> void*
        {
            expects(has_val(), "value is null.");

            return _impl.get_mut_mem();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto mem_as() const -> const type* requires(not typeinfo<type>::is_void) {
            return _impl.template mem_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto mut_mem_as() -> const type* requires(not typeinfo<type>::is_void) {
            return _impl.template mut_mem_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto check_mem_as() const -> const void* requires(not typeinfo<type>::is_void) {
            expects(has_val(), "value is null.");

            return _impl.template get_mem_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto check_mut_mem_as() -> void* requires(not typeinfo<type>::is_void) {
            expects(has_val(), "value is null.");

            return _impl.template get_mut_mem_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto val_t() const -> const std::type_info&
        {
            return _impl.get_val_t();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto val_size() const -> usize
        {
            return _impl.obj_size();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto has_val() const -> bool
        {
            return _impl.has_val();
        }

    protected:
        _impl_t _impl;
    };
}

/// ------------------------------------------------------------------------------------------------
/// final types
/// ------------------------------------------------------------------------------------------------
namespace atom
{
    template <typename value_t, usize buf_size = 50, typename allocator_t = default_mem_allocator>
    class box;

    template <typename value_t, usize buf_size = 50, typename allocator_t = default_mem_allocator>
    class copy_box;

    template <typename value_t, bool allow_non_move = true, usize buf_size = 50,
        typename allocator_t = default_mem_allocator>
    class move_box;

    template <typename value_t, bool allow_non_move = true, usize buf_size = 50,
        typename allocator_t = default_mem_allocator>
    class copy_move_box;

    template <typename value_t, usize buf_size, typename allocator_t>
    class box: public box_functions<_box_impl<value_t, false, false, false, buf_size, allocator_t>>
    {
        using this_t = box<value_t, buf_size, allocator_t>;
        using base_t =
            box_functions<_box_impl<value_t, false, false, false, buf_size, allocator_t>>;
        using _impl_t = typename base_t::_impl_t;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr box()
            : base_t()
        {}

        /// ----------------------------------------------------------------------------------------
        /// # null constructor
        /// ----------------------------------------------------------------------------------------
        constexpr box(const this_t& that) = delete;

        /// ----------------------------------------------------------------------------------------
        /// # copy operator
        /// ----------------------------------------------------------------------------------------
        constexpr this_t& operator=(const this_t& that) = delete;

        /// ----------------------------------------------------------------------------------------
        /// # move constructor
        /// ----------------------------------------------------------------------------------------
        constexpr box(this_t&& that) = delete;

        /// ----------------------------------------------------------------------------------------
        /// # move operator
        /// ----------------------------------------------------------------------------------------
        constexpr this_t& operator=(this_t&& that) = delete;

        /// ----------------------------------------------------------------------------------------
        /// # template copy constructor
        /// ----------------------------------------------------------------------------------------
        template <typename type, usize that_buf_size, typename that_allocator_t>
        constexpr box(const copy_box<type, that_buf_size, that_allocator_t>& that)
            requires typeinfo<value_t>::is_void
                     or (typeinfo<type>::template is_same_or_derived_from<value_t>)
            : base_t(typename _impl_t::copy_tag(), that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # null assignment
        /// ----------------------------------------------------------------------------------------
        template <typename type, usize that_buf_size, typename that_allocator_t>
        constexpr this_t& operator=(const copy_box<type, that_buf_size, that_allocator_t>& that)
            requires typeinfo<value_t>::is_void
                     or (typeinfo<type>::template is_same_or_derived_from<value_t>)
        {
            _impl.copy_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # template copy constructor
        /// ----------------------------------------------------------------------------------------
        template <typename type, usize that_buf_size, typename that_allocator_t>
        constexpr box(const copy_move_box<type, true, that_buf_size, that_allocator_t>& that)
            requires typeinfo<value_t>::is_void
                     or (typeinfo<type>::template is_same_or_derived_from<value_t>)
            : base_t(typename _impl_t::copy_tag(), that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # template copy operator
        /// ----------------------------------------------------------------------------------------
        template <typename type, usize that_buf_size, typename that_allocator_t>
        constexpr this_t& operator=(
            const copy_move_box<type, true, that_buf_size, that_allocator_t>& that)
            requires typeinfo<value_t>::is_void
                     or (typeinfo<type>::template is_same_or_derived_from<value_t>)
        {
            _impl.copy_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # template move constructor
        /// ----------------------------------------------------------------------------------------
        template <typename type, usize that_buf_size, typename that_allocator_t>
        constexpr box(move_box<type, true, that_buf_size, that_allocator_t>&& that)
            requires typeinfo<value_t>::is_void
                     or (typeinfo<type>::template is_same_or_derived_from<value_t>)
            : base_t(typename _impl_t::move_tag(), that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # template move operator
        /// ----------------------------------------------------------------------------------------
        template <typename type, usize that_buf_size, typename that_allocator_t>
        constexpr this_t& operator=(move_box<type, true, that_buf_size, that_allocator_t>&& that)
            requires(typeinfo<value_t>::is_void)
                    or (typeinfo<type>::template is_same_or_derived_from<value_t>)
        {
            _impl.move_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # template move constructor
        /// ----------------------------------------------------------------------------------------
        template <typename type, usize that_buf_size, typename that_allocator_t>
        constexpr box(copy_move_box<type, true, that_buf_size, that_allocator_t>&& that)
            requires typeinfo<value_t>::is_void
                     or (typeinfo<type>::template is_same_or_derived_from<value_t>)
            : base_t(typename _impl_t::move_tag(), that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # template move operator
        /// ----------------------------------------------------------------------------------------
        template <typename type, usize that_buf_size, typename that_allocator_t>
        constexpr this_t& operator=(
            copy_move_box<type, true, that_buf_size, that_allocator_t>&& that)
            requires typeinfo<value_t>::is_void
                     or (typeinfo<type>::template is_same_or_derived_from<value_t>)
        {
            _impl.move_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # constructor
        ///
        /// initializes with object.
        /// ----------------------------------------------------------------------------------------
        template <typename type, typename... arg_ts>
        constexpr box(type_holder<type> arg_t, arg_ts&&... args)
            requires(typeinfo<value_t>::is_void
                        or (typeinfo<type>::template is_same_or_derived_from<value_t>))
                    and (typeinfo<type>::template is_constructible_from<arg_ts...>)
            : base_t(arg_t, forward<arg_ts>(args)...)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # constructor
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr box(type&& obj)
            requires typeinfo<value_t>::is_void
                     or (typeinfo<type>::template is_same_or_derived_from<value_t>)
            : base_t(forward<type>(obj))
        {}

        /// ----------------------------------------------------------------------------------------
        /// # operator
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr this_t& operator=(type&& obj)
            requires typeinfo<value_t>::is_void
                     or (typeinfo<type>::template is_same_or_derived_from<value_t>)
        {
            _impl.set_val(forward<type>(obj));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # copy constructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~box() {}

    private:
        using base_t::_impl;
    };

    template <typename value_t, usize buf_size, typename allocator_t>
    class copy_box
        : public box_functions<_box_impl<value_t, true, false, false, buf_size, allocator_t>>
    {
        using this_t = copy_box<value_t, buf_size, allocator_t>;
        using base_t = box_functions<_box_impl<value_t, true, false, false, buf_size, allocator_t>>;
        using _impl_t = typename base_t::_impl_t;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr copy_box()
            : base_t()
        {}

        /// ----------------------------------------------------------------------------------------
        /// # copy constructor
        /// ----------------------------------------------------------------------------------------
        constexpr copy_box(const this_t& that)
            : base_t(typename _impl_t::copy_tag(), that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # copy assignment
        /// ----------------------------------------------------------------------------------------
        constexpr this_t& operator=(const this_t& that)
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
        constexpr copy_box(this_t&& that) = delete;

        /// ----------------------------------------------------------------------------------------
        /// # move operator
        /// ----------------------------------------------------------------------------------------
        constexpr this_t& operator=(this_t&& that) = delete;

        /// ----------------------------------------------------------------------------------------
        /// # template copy constructor
        /// ----------------------------------------------------------------------------------------
        template <typename type, bool allow_non_move, usize that_buf_size,
            typename that_allocator_t>
        constexpr copy_box(
            const copy_move_box<type, allow_non_move, that_buf_size, that_allocator_t>& that)
            : base_t(typename _impl_t::copy_tag(), that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # template copy operator
        /// ----------------------------------------------------------------------------------------
        template <typename type, bool allow_non_move, usize that_buf_size,
            typename that_allocator_t>
        constexpr this_t& operator=(
            const copy_move_box<type, allow_non_move, that_buf_size, that_allocator_t>& that)
        {
            _impl.copy_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # constructor
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr copy_box(type&& obj)
            : base_t(forward<type>(obj))
        {}

        /// ----------------------------------------------------------------------------------------
        /// # operator
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr this_t& operator=(type&& obj)
            requires typeinfo<type>::is_copyable
        {
            _impl.set_val(forward<type>(obj));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~copy_box() {}

    private:
        using base_t::_impl;
    };

    template <typename value_t, bool allow_non_move, usize buf_size, typename allocator_t>
    class move_box
        : public box_functions<
              _box_impl<value_t, false, true, allow_non_move, buf_size, allocator_t>>
    {
        using this_t = move_box<value_t, allow_non_move, buf_size, allocator_t>;
        using base_t =
            box_functions<_box_impl<value_t, false, true, allow_non_move, buf_size, allocator_t>>;
        using _impl_t = typename base_t::_impl_t;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr move_box()
            : base_t()
        {}

        /// ----------------------------------------------------------------------------------------
        /// # copy constructor
        /// ----------------------------------------------------------------------------------------
        constexpr move_box(const this_t& that) = delete;

        /// ----------------------------------------------------------------------------------------
        /// # copy operator
        /// ----------------------------------------------------------------------------------------
        constexpr move_box& operator=(const this_t& that) = delete;

        /// ----------------------------------------------------------------------------------------
        /// # template copy constructor
        /// ----------------------------------------------------------------------------------------
        template <typename type, usize that_buf_size, typename that_allocator_t>
            requires allow_non_move
        constexpr move_box(const copy_box<type, that_buf_size, that_allocator_t>& that)
            : base_t(typename _impl_t::copy_tag(), that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # template copy operator
        /// ----------------------------------------------------------------------------------------
        template <typename type, usize that_buf_size, typename that_allocator_t>
            requires allow_non_move
        constexpr move_box& operator=(const copy_box<type, that_buf_size, that_allocator_t>& that)
        {
            _impl.move_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # template copy constructor
        /// ----------------------------------------------------------------------------------------
        template <typename type, usize that_buf_size, typename that_allocator_t>
            requires allow_non_move
        constexpr move_box(
            const copy_move_box<type, allow_non_move, that_buf_size, that_allocator_t>& that)
            : base_t(typename _impl_t::copy_tag(), that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # template copy operator
        /// ----------------------------------------------------------------------------------------
        template <typename type, usize that_buf_size, typename that_allocator_t>
            requires allow_non_move
        constexpr move_box& operator=(
            const copy_move_box<type, allow_non_move, that_buf_size, that_allocator_t>& that)
        {
            _impl.move_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # move constructor
        /// ----------------------------------------------------------------------------------------
        constexpr move_box(this_t&& that)
            : base_t(typename _impl_t::move_tag(), that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # move assignment
        /// ----------------------------------------------------------------------------------------
        constexpr move_box& operator=(this_t&& that)
        {
            _move_box(mov(other));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # move assignment
        /// ----------------------------------------------------------------------------------------
        template <typename type, usize that_buf_size, typename that_allocator_t>
        constexpr move_box(
            copy_move_box<type, allow_non_move, that_buf_size, that_allocator_t>&& that)
            : base_t(typename _impl_t::move_tag(), that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # template move operator
        /// ----------------------------------------------------------------------------------------
        template <typename type, usize that_buf_size, typename that_allocator_t>
        constexpr move_box& operator=(
            copy_move_box<type, allow_non_move, that_buf_size, that_allocator_t>&& that)
        {
            _impl.move_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # constructor
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr move_box(type&& obj)
            : base_t(forward<type>(obj))
        {}

        /// ----------------------------------------------------------------------------------------
        /// # operator
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr move_box& operator=(type&& obj)
        {
            _impl.set_val(forward<type>(obj));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~move_box() {}

    private:
        using base_t::_impl;
    };

    template <typename value_t, bool allow_non_move, usize buf_size, typename allocator_t>
    class copy_move_box
        : public box_functions<
              _box_impl<value_t, true, true, allow_non_move, buf_size, allocator_t>>
    {
        using this_t = copy_move_box<value_t, allow_non_move, buf_size, allocator_t>;
        using base_t =
            box_functions<_box_impl<value_t, true, true, allow_non_move, buf_size, allocator_t>>;
        using _impl_t = typename base_t::_impl_t;

    private:
        using base_t::_impl;

    public:
        /// ----------------------------------------------------------------------------------------
        /// sets the new object.
        /// ----------------------------------------------------------------------------------------
        constexpr copy_move_box()
            : base_t()
        {}

        /// ----------------------------------------------------------------------------------------
        /// # copy constructor
        /// ----------------------------------------------------------------------------------------
        constexpr copy_move_box(const this_t& that)
            : base_t(typename _impl_t::copy_tag(), that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # copy operator
        /// ----------------------------------------------------------------------------------------
        constexpr copy_move_box& operator=(const this_t& that)
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
        template <typename type, usize that_buf_size, typename that_allocator_t>
            requires allow_non_move
        constexpr copy_move_box(const copy_box<type, that_buf_size, that_allocator_t>& that)
            : base_t(typename _impl_t::copy_tag(), that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// memory allocator.
        /// ----------------------------------------------------------------------------------------
        template <typename type, usize that_buf_size, typename that_allocator_t>
            requires allow_non_move
        constexpr copy_move_box& operator=(
            const copy_box<type, that_buf_size, that_allocator_t>& that)
        {
            _impl.copy_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// heap memory allocated.
        /// ----------------------------------------------------------------------------------------
        template <typename type, usize that_buf_size, typename that_allocator_t>
            requires allow_non_move
        constexpr copy_move_box(
            const copy_move_box<type, allow_non_move, that_buf_size, that_allocator_t>& that)
            : base_t(typename _impl_t::copy_tag(), that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// size of heap memory allocated.
        /// ----------------------------------------------------------------------------------------
        template <typename type, usize that_buf_size, typename that_allocator_t>
            requires allow_non_move
        constexpr copy_move_box& operator=(
            const copy_move_box<type, allow_non_move, that_buf_size, that_allocator_t>& that)
        {
            _impl.copy_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// object data.
        /// ----------------------------------------------------------------------------------------
        constexpr copy_move_box(this_t&& that)
            : base_t(typename _impl_t::move_tag(), that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # move operator
        /// ----------------------------------------------------------------------------------------
        constexpr copy_move_box& operator=(this_t&& that)
        {
            _impl.move_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # template move constructor
        /// ----------------------------------------------------------------------------------------
        template <typename type, bool that_allow_non_move, usize that_buf_size,
            typename that_allocator_t>
            requires allow_non_move
        constexpr copy_move_box(
            move_box<type, that_allow_non_move, that_buf_size, that_allocator_t>&& that)
            : base_t(typename _impl_t::move_tag(), that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # template move operator
        /// ----------------------------------------------------------------------------------------
        template <typename type, bool that_allow_non_move, usize that_buf_size,
            typename that_allocator_t>
            requires allow_non_move
        constexpr copy_move_box& operator=(
            move_box<type, that_allow_non_move, that_buf_size, that_allocator_t>&& that)
        {
            _impl.move_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # template move constructor
        /// ----------------------------------------------------------------------------------------
        template <typename type, bool that_allow_non_move, usize that_buf_size,
            typename that_allocator_t>
            requires allow_non_move
        constexpr copy_move_box(
            copy_move_box<type, that_allow_non_move, that_buf_size, that_allocator_t>&& that)
            : base_t(typename _impl_t::move_tag(), that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # template move operator
        /// ----------------------------------------------------------------------------------------
        template <typename type, bool that_allow_non_move, usize that_buf_size,
            typename that_allocator_t>
            requires allow_non_move
        constexpr copy_move_box& operator=(
            copy_move_box<type, that_allow_non_move, that_buf_size, that_allocator_t>&& that)
        {
            _impl.move_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # constructor
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr copy_move_box(type&& obj)
            : base_t(forward<type>(obj))
        {}

        /// ----------------------------------------------------------------------------------------
        /// # operator
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr copy_move_box& operator=(type&& obj)
        {
            _impl.set_val(forward<type>(obj));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~copy_move_box() {}
    };
}
