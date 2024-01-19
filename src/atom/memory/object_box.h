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
    protected:
        using _impl_type = _timpl_;

    public:
        using tval = typename _impl_type::tval;

    public:
        template <typename... arg_types>
        constexpr box_functions(arg_types&&... args)
            : _impl{ forward<arg_types>(args)... }
        {}

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename t, typename... targs>
        constexpr auto emplace(targs&&... args) -> t&
            requires rsame_or_derived_from<tpure<t>, tval>
        {
            _impl.template emplace_val<t>(forward<targs>(args)...);
            return _impl.template get_mut_val_as<t>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename t>
        constexpr auto set(t&& obj) -> t&
            requires rsame_or_derived_from<tpure<t>, tval>
        {
            _impl._set_val(forward<t>(obj));
            return _impl.template get_mut_val_as<t>();
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
        constexpr auto get() const -> const tval&
        {
            debug_expects(has_val(), "value is null.");

            return _impl.get_val();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_mut() -> tval&
        {
            debug_expects(has_val(), "value is null.");

            return _impl.get_val_mut();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto check_get() const -> const tval&
        {
            expects(has_val(), "value is null.");

            return _impl.get_val();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto check_get_mut() -> tval&
        {
            expects(has_val(), "value is null.");

            return _impl.get_val_mut();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename t>
        constexpr auto get_as() const -> const t&
            requires rsame_or_derived_from<tpure<t>, tval>
        {
            debug_expects(has_val(), "value is null.");

            return _impl.template get_val_as<t>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename t>
        constexpr auto get_mut_as() -> t&
            requires rsame_or_derived_from<tpure<t>, tval>
        {
            debug_expects(has_val(), "value is null.");

            return _impl.template get_mut_val_as<t>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename t>
        constexpr auto check_get_as() const -> const t&
            requires rsame_or_derived_from<tpure<t>, tval>
        {
            expects(has_val(), "value is null.");

            return _impl.template get_val_as<t>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename t>
        constexpr auto check_get_mut_as() -> t&
            requires rsame_or_derived_from<tpure<t>, tval>
        {
            expects(has_val(), "value is null.");

            return _impl.template get_mut_val_as<t>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto mem() const -> const ptr<tval>
        {
            return _impl.mem();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto mut_mem() -> ptr<tval>
        {
            return _impl.mut_mem();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto check_mem() const -> const ptr<tval>
        {
            expects(has_val(), "value is null.");

            return _impl.mem();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto check_mut_mem() -> ptr<tval>
        {
            expects(has_val(), "value is null.");

            return _impl.mut_mem();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename t>
        constexpr auto mem_as() const -> const ptr<t>
            requires rsame_or_derived_from<tpure<t>, tval>
        {
            return _impl.template mem_as<t>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename t>
        constexpr auto mut_mem_as() -> ptr<t>
            requires rsame_or_derived_from<tpure<t>, tval>
        {
            return _impl.template mut_mem_as<t>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename t>
        constexpr auto check_mem_as() const -> const ptr<t>
            requires rsame_or_derived_from<tpure<t>, tval>
        {
            expects(has_val(), "value is null.");

            return _impl.template mem_as<t>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename t>
        constexpr auto check_mut_mem_as() -> ptr<t>
            requires rsame_or_derived_from<tpure<t>, tval>
        {
            expects(has_val(), "value is null.");

            return _impl.template mut_mem_as<t>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto val_type() const -> const type_info&
        {
            return _impl.obj_type();
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
        _impl_type _impl;
    };

    template <typename _timpl_>
        requires(ris_void<typename _timpl_::tval>)
    class box_functions<_timpl_>
    {
    protected:
        using _impl_type = _timpl_;

    public:
        using tval = void;

    public:
        template <typename... targs>
        constexpr box_functions(targs&&... args)
            : _impl{ forward<targs>(args)... }
        {}

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename t, typename... targs>
        constexpr auto emplace(targs&&... args) -> t&
            requires(not ris_void<t>)
        {
            _impl.template emplace_val<t>(forward<targs>(args)...);
            return _impl.template get_mut_val_as<t>();
        }

        /// ----------------------------------------------------------------------------------------
        /// requirements for `object` accepted by this `object_box`.
        /// ----------------------------------------------------------------------------------------
        template <typename t>
        constexpr auto set(t&& obj) -> t&
        {
            _impl._set_val(forward<t>(obj));
            return _impl.template get_mut_val_as<t>();
        }

        /// ----------------------------------------------------------------------------------------
        /// requirements for other `object_box` accepted by this `object_box`.
        /// for example, `copy_constructor` and `move_constructor`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto destroy()
        {
            _impl.destroy_val();
        }

            // if this {box} is `movable` the {other_box} should also be `movable` unless
            // `non_movable_object` is allowed.
            requires !movable || (other_movable || allow_non_movable_object);

            return _impl.template get_val_as<t>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename t>
        constexpr auto get_mut_as() -> t&
        {
            debug_expects(has_val(), "value is null.");

            return _impl.template get_mut_val_as<t>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename t>
        constexpr auto check_get_as() const -> const t&
        {
            expects(has_val(), "value is null.");

            return _impl.template get_val_as<t>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename t>
        constexpr auto check_get_mut_as() -> t&
        {
            expects(has_val(), "value is null.");

            return _impl.template get_mut_val_as<t>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto mem() const -> mem_ptr<void>
        {
            return _impl.get_mem();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto mut_mem() -> mut_mem_ptr<void>
        {
            return _impl.get_mut_mem();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto check_mem() const -> mem_ptr<void>
        {
            expects(has_val(), "value is null.");

            return _impl.get_mem();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto check_mut_mem() -> mut_mem_ptr<void>
        {
            expects(has_val(), "value is null.");

            return _impl.get_mut_mem();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename t>
        constexpr auto mem_as() const -> const ptr<t>
            requires(not ris_void<t>)
        {
            return _impl.template mem_as<t>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename t>
        constexpr auto mut_mem_as() -> ptr<t>
            requires(not ris_void<t>)
        {
            return _impl.template mut_mem_as<t>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename t>
        constexpr auto check_mem_as() const -> mem_ptr<void>
            requires(not ris_void<t>)
        {
            expects(has_val(), "value is null.");

            return _impl.template get_mem_as<t>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename t>
        constexpr auto check_mut_mem_as() -> mut_mem_ptr<void>
            requires(not ris_void<t>)
        {
            expects(has_val(), "value is null.");

            return _impl.template get_mut_mem_as<t>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto val_type() const -> const type_info&
        {
            return _impl.get_val_type();
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
        _impl_type _impl;
    };

    template <typename tval, usize buf_size = 50, typename talloc = default_mem_allocator>
    class box;

    template <typename tval, usize buf_size = 50, typename talloc = default_mem_allocator>
    class copy_box;

    template <typename tval, bool allow_non_move = true, usize buf_size = 50,
        typename talloc = default_mem_allocator>
    class move_box;

    template <typename tval, bool allow_non_move = true, usize buf_size = 50,
        typename talloc = default_mem_allocator>
    class copy_move_box;

    template <typename tval, usize buf_size, typename talloc>
    class box: public box_functions<_box_impl<tval, false, false, false, buf_size, talloc>>
    {
        using this_type = box<tval, buf_size, talloc>;
        using base_type = box_functions<_box_impl<tval, false, false, false, buf_size, talloc>>;
        using _impl_type = typename base_type::_impl_type;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr box()
            : base_type{}
        {}

        /// ----------------------------------------------------------------------------------------
        /// # null constructor
        /// ----------------------------------------------------------------------------------------
        constexpr box(const this_type& that) = delete;

        /// ----------------------------------------------------------------------------------------
        /// # copy operator
        /// ----------------------------------------------------------------------------------------
        constexpr this_type& operator=(const this_type& that) = delete;

        /// ----------------------------------------------------------------------------------------
        /// # move constructor
        /// ----------------------------------------------------------------------------------------
        constexpr box(this_type&& that) = delete;

        /// ----------------------------------------------------------------------------------------
        /// # move operator
        /// ----------------------------------------------------------------------------------------
        constexpr this_type& operator=(this_type&& that) = delete;

        /// ----------------------------------------------------------------------------------------
        /// # template copy constructor
        /// ----------------------------------------------------------------------------------------
        template <typename t, usize that_buf_size, typename tthat_alloc>
        constexpr box(const copy_box<t, that_buf_size, tthat_alloc>& that)
            requires ris_void<tval> or rsame_or_derived_from<t, tval>
            : base_type{ typename _impl_type::copy_tag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # null assignment
        /// ----------------------------------------------------------------------------------------
        template <typename t, usize that_buf_size, typename tthat_alloc>
        constexpr this_type& operator=(const copy_box<t, that_buf_size, tthat_alloc>& that)
            requires ris_void<tval> or rsame_or_derived_from<t, tval>
        {
            _impl.copy_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # template copy constructor
        /// ----------------------------------------------------------------------------------------
        template <typename t, usize that_buf_size, typename tthat_alloc>
        constexpr box(const copy_move_box<t, true, that_buf_size, tthat_alloc>& that)
            requires ris_void<tval> or rsame_or_derived_from<t, tval>
            : base_type{ typename _impl_type::copy_tag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # template copy operator
        /// ----------------------------------------------------------------------------------------
        template <typename t, usize that_buf_size, typename tthat_alloc>
        constexpr this_type& operator=(
            const copy_move_box<t, true, that_buf_size, tthat_alloc>& that)
            requires ris_void<tval> or rsame_or_derived_from<t, tval>
        {
            _impl.copy_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # template move constructor
        /// ----------------------------------------------------------------------------------------
        template <typename t, usize that_buf_size, typename tthat_alloc>
        constexpr box(move_box<t, true, that_buf_size, tthat_alloc>&& that)
            requires ris_void<tval> or rsame_or_derived_from<t, tval>
            : base_type{ typename _impl_type::move_tag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # template move operator
        /// ----------------------------------------------------------------------------------------
        template <typename t, usize that_buf_size, typename tthat_alloc>
        constexpr this_type& operator=(move_box<t, true, that_buf_size, tthat_alloc>&& that)
            requires(ris_void<tval>) or rsame_or_derived_from<t, tval>
        {
            _impl.move_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # template move constructor
        /// ----------------------------------------------------------------------------------------
        template <typename t, usize that_buf_size, typename tthat_alloc>
        constexpr box(copy_move_box<t, true, that_buf_size, tthat_alloc>&& that)
            requires ris_void<tval> or rsame_or_derived_from<t, tval>
            : base_type{ typename _impl_type::move_tag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # template move operator
        /// ----------------------------------------------------------------------------------------
        template <typename t, usize that_buf_size, typename tthat_alloc>
        constexpr this_type& operator=(copy_move_box<t, true, that_buf_size, tthat_alloc>&& that)
            requires ris_void<tval> or rsame_or_derived_from<t, tval>
        {
            _impl.move_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # constructor
        ///
        /// initializes with object.
        /// ----------------------------------------------------------------------------------------
        template <typename t, typename... targs>
        constexpr box(type_holder<t> targ, targs&&... args)
            requires(ris_void<tval> or rsame_or_derived_from<t, tval>)
                    and rconstructible<t, targs...>
            : base_type{ targ, forward<targs>(args)... }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # constructor
        /// ----------------------------------------------------------------------------------------
        template <typename t>
        constexpr box(t&& obj)
            requires ris_void<tval> or rsame_or_derived_from<tpure<t>, tval>
            : base_type{ forward<t>(obj) }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # operator
        /// ----------------------------------------------------------------------------------------
        template <typename t>
        constexpr this_type& operator=(t&& obj)
            requires ris_void<tval> or rsame_or_derived_from<tpure<t>, tval>
        {
            _impl.set_val(forward<t>(obj));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # copy constructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~box() {}

    private:
        using base_type::_impl;
    };

    template <typename tval, usize buf_size, typename talloc>
    class copy_box: public box_functions<_box_impl<tval, true, false, false, buf_size, talloc>>
    {
        using this_type = copy_box<tval, buf_size, talloc>;
        using base_type = box_functions<_box_impl<tval, true, false, false, buf_size, talloc>>;
        using _impl_type = typename base_type::_impl_type;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr copy_box()
            : base_type{}
        {}

        /// ----------------------------------------------------------------------------------------
        /// # copy constructor
        /// ----------------------------------------------------------------------------------------
        constexpr copy_box(const this_type& that)
            : base_type{ typename _impl_type::copy_tag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # copy assignment
        /// ----------------------------------------------------------------------------------------
        constexpr this_type& operator=(const this_type& that)
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
        constexpr copy_box(this_type&& that) = delete;

        /// ----------------------------------------------------------------------------------------
        /// # move operator
        /// ----------------------------------------------------------------------------------------
        constexpr this_type& operator=(this_type&& that) = delete;

        /// ----------------------------------------------------------------------------------------
        /// # template copy constructor
        /// ----------------------------------------------------------------------------------------
        template <typename t, bool allow_non_move, usize that_buf_size, typename tthat_alloc>
        constexpr copy_box(const copy_move_box<t, allow_non_move, that_buf_size, tthat_alloc>& that)
            : base_type{ typename _impl_type::copy_tag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # template copy operator
        /// ----------------------------------------------------------------------------------------
        template <typename t, bool allow_non_move, usize that_buf_size, typename tthat_alloc>
        constexpr this_type& operator=(
            const copy_move_box<t, allow_non_move, that_buf_size, tthat_alloc>& that)
        {
            _impl.copy_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # constructor
        /// ----------------------------------------------------------------------------------------
        template <typename t>
        constexpr copy_box(t&& obj)
            : base_type{ forward<t>(obj) }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # operator
        /// ----------------------------------------------------------------------------------------
        template <typename t>
        constexpr this_type& operator=(t&& obj)
            requires(rcopyable<t>)
        {
            _impl.set_val(forward<t>(obj));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~copy_box() {}

    private:
        using base_type::_impl;
    };

    template <typename tval, bool allow_non_move, usize buf_size, typename talloc>
    class move_box
        : public box_functions<_box_impl<tval, false, true, allow_non_move, buf_size, talloc>>
    {
        using this_type = move_box<tval, allow_non_move, buf_size, talloc>;
        using base_type =
            box_functions<_box_impl<tval, false, true, allow_non_move, buf_size, talloc>>;
        using _impl_type = typename base_type::_impl_type;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr move_box()
            : base_type{}
        {}

        /// ----------------------------------------------------------------------------------------
        /// # copy constructor
        /// ----------------------------------------------------------------------------------------
        constexpr move_box(const this_type& that) = delete;

        /// ----------------------------------------------------------------------------------------
        /// # copy operator
        /// ----------------------------------------------------------------------------------------
        constexpr move_box& operator=(const this_type& that) = delete;

        /// ----------------------------------------------------------------------------------------
        /// # template copy constructor
        /// ----------------------------------------------------------------------------------------
        template <typename t, usize that_buf_size, typename tthat_alloc>
            requires allow_non_move
        constexpr move_box(const copy_box<t, that_buf_size, tthat_alloc>& that)
            : base_type{ typename _impl_type::copy_tag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # template copy operator
        /// ----------------------------------------------------------------------------------------
        template <typename t, usize that_buf_size, typename tthat_alloc>
            requires allow_non_move
        constexpr move_box& operator=(const copy_box<t, that_buf_size, tthat_alloc>& that)
        {
            _impl.move_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # template copy constructor
        /// ----------------------------------------------------------------------------------------
        template <typename t, usize that_buf_size, typename tthat_alloc>
            requires allow_non_move
        constexpr move_box(const copy_move_box<t, allow_non_move, that_buf_size, tthat_alloc>& that)
            : base_type{ typename _impl_type::copy_tag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # template copy operator
        /// ----------------------------------------------------------------------------------------
        template <typename t, usize that_buf_size, typename tthat_alloc>
            requires allow_non_move
        constexpr move_box& operator=(
            const copy_move_box<t, allow_non_move, that_buf_size, tthat_alloc>& that)
        {
            _impl.move_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # move constructor
        /// ----------------------------------------------------------------------------------------
        constexpr move_box(this_type&& that)
            : base_type{ typename _impl_type::move_tag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # move assignment
        /// ----------------------------------------------------------------------------------------
        constexpr move_box& operator=(this_type&& that)
        {
            _move_box(mov(other));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # move assignment
        /// ----------------------------------------------------------------------------------------
        template <typename t, usize that_buf_size, typename tthat_alloc>
        constexpr move_box(copy_move_box<t, allow_non_move, that_buf_size, tthat_alloc>&& that)
            : base_type{ typename _impl_type::move_tag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # template move operator
        /// ----------------------------------------------------------------------------------------
        template <typename t, usize that_buf_size, typename tthat_alloc>
        constexpr move_box& operator=(
            copy_move_box<t, allow_non_move, that_buf_size, tthat_alloc>&& that)
        {
            _impl.move_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # constructor
        /// ----------------------------------------------------------------------------------------
        template <typename t>
        constexpr move_box(t&& obj)
            : base_type{ forward<t>(obj) }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # operator
        /// ----------------------------------------------------------------------------------------
        template <typename t>
        constexpr move_box& operator=(t&& obj)
        {
            _impl.set_val(forward<t>(obj));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~move_box() {}

    private:
        using base_type::_impl;
    };

    template <typename tval, bool allow_non_move, usize buf_size, typename talloc>
    class copy_move_box
        : public box_functions<_box_impl<tval, true, true, allow_non_move, buf_size, talloc>>
    {
        using this_type = copy_move_box<tval, allow_non_move, buf_size, talloc>;
        using base_type =
            box_functions<_box_impl<tval, true, true, allow_non_move, buf_size, talloc>>;
        using _impl_type = typename base_type::_impl_type;

    private:
        using base_type::_impl;

    public:
        /// ----------------------------------------------------------------------------------------
        /// sets the new object.
        /// ----------------------------------------------------------------------------------------
        constexpr copy_move_box()
            : base_type{}
        {}

        /// ----------------------------------------------------------------------------------------
        /// # copy constructor
        /// ----------------------------------------------------------------------------------------
        constexpr copy_move_box(const this_type& that)
            : base_type{ typename _impl_type::copy_tag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # copy operator
        /// ----------------------------------------------------------------------------------------
        constexpr copy_move_box& operator=(const this_type& that)
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
        template <typename t, usize that_buf_size, typename tthat_alloc>
            requires allow_non_move
        constexpr copy_move_box(const copy_box<t, that_buf_size, tthat_alloc>& that)
            : base_type{ typename _impl_type::copy_tag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// memory allocator.
        /// ----------------------------------------------------------------------------------------
        allocator_type _allocator;

        /// ----------------------------------------------------------------------------------------
        /// heap memory allocated.
        /// ----------------------------------------------------------------------------------------
        template <typename t, usize that_buf_size, typename tthat_alloc>
            requires allow_non_move
        constexpr copy_move_box(
            const copy_move_box<t, allow_non_move, that_buf_size, tthat_alloc>& that)
            : base_type{ typename _impl_type::copy_tag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// size of heap memory allocated.
        /// ----------------------------------------------------------------------------------------
        usize _heap_mem_size;

        /// ----------------------------------------------------------------------------------------
        /// object data.
        /// ----------------------------------------------------------------------------------------
        constexpr copy_move_box(this_type&& that)
            : base_type{ typename _impl_type::move_tag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # move operator
        /// ----------------------------------------------------------------------------------------
        constexpr copy_move_box& operator=(this_type&& that)
        {
            _impl.move_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # template move constructor
        /// ----------------------------------------------------------------------------------------
        template <typename t, bool that_allow_non_move, usize that_buf_size, typename tthat_alloc>
            requires allow_non_move
        constexpr copy_move_box(move_box<t, that_allow_non_move, that_buf_size, tthat_alloc>&& that)
            : base_type{ typename _impl_type::move_tag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # template move operator
        /// ----------------------------------------------------------------------------------------
        template <typename t, bool that_allow_non_move, usize that_buf_size, typename tthat_alloc>
            requires allow_non_move
        constexpr copy_move_box& operator=(
            move_box<t, that_allow_non_move, that_buf_size, tthat_alloc>&& that)
        {
            _impl.move_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # template move constructor
        /// ----------------------------------------------------------------------------------------
        template <typename t, bool that_allow_non_move, usize that_buf_size, typename tthat_alloc>
            requires allow_non_move
        constexpr copy_move_box(
            copy_move_box<t, that_allow_non_move, that_buf_size, tthat_alloc>&& that)
            : base_type{ typename _impl_type::move_tag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # template move operator
        /// ----------------------------------------------------------------------------------------
        template <typename t, bool that_allow_non_move, usize that_buf_size, typename tthat_alloc>
            requires allow_non_move
        constexpr copy_move_box& operator=(
            copy_move_box<t, that_allow_non_move, that_buf_size, tthat_alloc>&& that)
        {
            _impl.move_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # constructor
        /// ----------------------------------------------------------------------------------------
        template <typename t>
        constexpr copy_move_box(t&& obj)
            : base_type{ forward<t>(obj) }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # operator
        /// ----------------------------------------------------------------------------------------
        template <typename t>
        constexpr copy_move_box& operator=(t&& obj)
        {
            _impl.set_val(forward<t>(obj));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~copy_move_box() {}
    };
}
