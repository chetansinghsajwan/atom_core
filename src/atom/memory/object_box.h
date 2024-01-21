#pragma once
// #include "atom/core/requirements.h"
// #include "atom/memory/default_mem_allocator.h"
// #include "atom/tti.h"
#include "_box_impl.h"

namespace atom
{
    namespace internal
    {
    protected:
        using _impl_type = _timpl_;

    public:
        using value_type = typename _impl_type::value_type;

    public:
        template <typename... arg_types>
        constexpr box_functions(arg_types&&... args)
            : _impl{ forward<arg_types>(args)... }
        {}

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type, typename... arg_types>
        constexpr auto emplace(arg_types&&... args) -> type&
            requires rsame_or_derived_from<pure_type<type>, value_type>
        {
            _impl.template emplace_val<type>(forward<arg_types>(args)...);
            return _impl.template get_mut_val_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto set(type&& obj) -> type&
            requires rsame_or_derived_from<pure_type<type>, value_type>
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
        constexpr auto get() const -> const value_type&
        {
            debug_expects(has_val(), "value is null.");

            return _impl.get_val();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_mut() -> value_type&
        {
            debug_expects(has_val(), "value is null.");

            return _impl.get_val_mut();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto check_get() const -> const value_type&
        {
            expects(has_val(), "value is null.");

            return _impl.get_val();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto check_get_mut() -> value_type&
        {
            expects(has_val(), "value is null.");

            return _impl.get_val_mut();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto get_as() const -> const type&
            requires rsame_or_derived_from<pure_type<type>, value_type>
        {
            debug_expects(has_val(), "value is null.");

            return _impl.template get_val_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto get_mut_as() -> type&
            requires rsame_or_derived_from<pure_type<type>, value_type>
        {
            debug_expects(has_val(), "value is null.");

            return _impl.template get_mut_val_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto check_get_as() const -> const type&
            requires rsame_or_derived_from<pure_type<type>, value_type>
        {
            expects(has_val(), "value is null.");

            return _impl.template get_val_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto check_get_mut_as() -> type&
            requires rsame_or_derived_from<pure_type<type>, value_type>
        {
            expects(has_val(), "value is null.");

            return _impl.template get_mut_val_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto mem() const -> const ptr<value_type>
        {
            return _impl.mem();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto mut_mem() -> ptr<value_type>
        {
            return _impl.mut_mem();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto check_mem() const -> const ptr<value_type>
        {
            expects(has_val(), "value is null.");

            return _impl.mem();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto check_mut_mem() -> ptr<value_type>
        {
            expects(has_val(), "value is null.");

            return _impl.mut_mem();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto mem_as() const -> const ptr<type>
            requires rsame_or_derived_from<pure_type<type>, value_type>
        {
            return _impl.template mem_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto mut_mem_as() -> ptr<type>
            requires rsame_or_derived_from<pure_type<type>, value_type>
        {
            return _impl.template mut_mem_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto check_mem_as() const -> const ptr<type>
            requires rsame_or_derived_from<pure_type<type>, value_type>
        {
            expects(has_val(), "value is null.");

            return _impl.template mem_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto check_mut_mem_as() -> ptr<type>
            requires rsame_or_derived_from<pure_type<type>, value_type>
        {
            expects(has_val(), "value is null.");

            return _impl.template mut_mem_as<type>();
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
        requires(ris_void<typename _timpl_::value_type>)
    class box_functions<_timpl_>
    {
    protected:
        using _impl_type = _timpl_;

    public:
        using value_type = void;

    public:
        template <typename... arg_types>
        constexpr box_functions(arg_types&&... args)
            : _impl{ forward<arg_types>(args)... }
        {}

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type, typename... arg_types>
        constexpr auto emplace(arg_types&&... args) -> type&
            requires(not ris_void<type>)
        {
            _impl.template emplace_val<type>(forward<arg_types>(args)...);
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
            debug_expects(has_val(), "value is null.");

            return _impl.template get_val_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto get_mut_as() -> type&
        {
            debug_expects(has_val(), "value is null.");

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
        template <typename type>
        constexpr auto mem_as() const -> const ptr<type>
            requires(not ris_void<type>)
        {
            return _impl.template mem_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto mut_mem_as() -> ptr<type>
            requires(not ris_void<type>)
        {
            return _impl.template mut_mem_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto check_mem_as() const -> mem_ptr<void>
            requires(not ris_void<type>)
        {
            expects(has_val(), "value is null.");

            return _impl.template get_mem_as<type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr auto check_mut_mem_as() -> mut_mem_ptr<void>
            requires(not ris_void<type>)
        {
            expects(has_val(), "value is null.");

            return _impl.template get_mut_mem_as<type>();
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

    template <typename value_type, usize buf_size = 50, typename allocator_type = default_mem_allocator>
    class box;

    template <typename value_type, usize buf_size = 50, typename allocator_type = default_mem_allocator>
    class copy_box;

    template <typename value_type, bool allow_non_move = true, usize buf_size = 50,
        typename allocator_type = default_mem_allocator>
    class move_box;

    template <typename value_type, bool allow_non_move = true, usize buf_size = 50,
        typename allocator_type = default_mem_allocator>
    class copy_move_box;

    template <typename value_type, usize buf_size, typename allocator_type>
    class box
        : public box_functions<_box_impl<value_type, false, false, false, buf_size, allocator_type>>
    {
        using this_type = box<value_type, buf_size, allocator_type>;
        using base_type =
            box_functions<_box_impl<value_type, false, false, false, buf_size, allocator_type>>;
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
        template <typename type, usize that_buf_size, typename that_allocator_type>
        constexpr box(const copy_box<type, that_buf_size, that_allocator_type>& that)
            requires ris_void<value_type> or rsame_or_derived_from<type, value_type>
            : base_type{ typename _impl_type::copy_tag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # null assignment
        /// ----------------------------------------------------------------------------------------
        template <typename type, usize that_buf_size, typename that_allocator_type>
        constexpr this_type& operator=(const copy_box<type, that_buf_size, that_allocator_type>& that)
            requires ris_void<value_type> or rsame_or_derived_from<type, value_type>
        {
            _impl.copy_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # template copy constructor
        /// ----------------------------------------------------------------------------------------
        template <typename type, usize that_buf_size, typename that_allocator_type>
        constexpr box(const copy_move_box<type, true, that_buf_size, that_allocator_type>& that)
            requires ris_void<value_type> or rsame_or_derived_from<type, value_type>
            : base_type{ typename _impl_type::copy_tag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # template copy operator
        /// ----------------------------------------------------------------------------------------
        template <typename type, usize that_buf_size, typename that_allocator_type>
        constexpr this_type& operator=(
            const copy_move_box<type, true, that_buf_size, that_allocator_type>& that)
            requires ris_void<value_type> or rsame_or_derived_from<type, value_type>
        {
            _impl.copy_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # template move constructor
        /// ----------------------------------------------------------------------------------------
        template <typename type, usize that_buf_size, typename that_allocator_type>
        constexpr box(move_box<type, true, that_buf_size, that_allocator_type>&& that)
            requires ris_void<value_type> or rsame_or_derived_from<type, value_type>
            : base_type{ typename _impl_type::move_tag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # template move operator
        /// ----------------------------------------------------------------------------------------
        template <typename type, usize that_buf_size, typename that_allocator_type>
        constexpr this_type& operator=(move_box<type, true, that_buf_size, that_allocator_type>&& that)
            requires(ris_void<value_type>) or rsame_or_derived_from<type, value_type>
        {
            _impl.move_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # template move constructor
        /// ----------------------------------------------------------------------------------------
        template <typename type, usize that_buf_size, typename that_allocator_type>
        constexpr box(copy_move_box<type, true, that_buf_size, that_allocator_type>&& that)
            requires ris_void<value_type> or rsame_or_derived_from<type, value_type>
            : base_type{ typename _impl_type::move_tag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # template move operator
        /// ----------------------------------------------------------------------------------------
        template <typename type, usize that_buf_size, typename that_allocator_type>
        constexpr this_type& operator=(
            copy_move_box<type, true, that_buf_size, that_allocator_type>&& that)
            requires ris_void<value_type> or rsame_or_derived_from<type, value_type>
        {
            _impl.move_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # constructor
        ///
        /// initializes with object.
        /// ----------------------------------------------------------------------------------------
        template <typename type, typename... arg_types>
        constexpr box(type_holder<type> arg_type, arg_types&&... args)
            requires(ris_void<value_type> or rsame_or_derived_from<type, value_type>)
                    and rconstructible<type, arg_types...>
            : base_type{ arg_type, forward<arg_types>(args)... }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # constructor
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr box(type&& obj)
            requires ris_void<value_type> or rsame_or_derived_from<pure_type<type>, value_type>
            : base_type{ forward<type>(obj) }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # operator
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr this_type& operator=(type&& obj)
            requires ris_void<value_type> or rsame_or_derived_from<pure_type<type>, value_type>
        {
            _impl.set_val(forward<type>(obj));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # copy constructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~box() {}

    private:
        using base_type::_impl;
    };

    template <typename value_type, usize buf_size, typename allocator_type>
    class copy_box
        : public box_functions<_box_impl<value_type, true, false, false, buf_size, allocator_type>>
    {
        using this_type = copy_box<value_type, buf_size, allocator_type>;
        using base_type =
            box_functions<_box_impl<value_type, true, false, false, buf_size, allocator_type>>;
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
        template <typename type, bool allow_non_move, usize that_buf_size, typename that_allocator_type>
        constexpr copy_box(
            const copy_move_box<type, allow_non_move, that_buf_size, that_allocator_type>& that)
            : base_type{ typename _impl_type::copy_tag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # template copy operator
        /// ----------------------------------------------------------------------------------------
        template <typename type, bool allow_non_move, usize that_buf_size, typename that_allocator_type>
        constexpr this_type& operator=(
            const copy_move_box<type, allow_non_move, that_buf_size, that_allocator_type>& that)
        {
            _impl.copy_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # constructor
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr copy_box(type&& obj)
            : base_type{ forward<type>(obj) }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # operator
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr this_type& operator=(type&& obj)
            requires(rcopyable<type>)
        {
            _impl.set_val(forward<type>(obj));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~copy_box() {}

    private:
        using base_type::_impl;
    };

    template <typename value_type, bool allow_non_move, usize buf_size, typename allocator_type>
    class move_box
        : public box_functions<
              _box_impl<value_type, false, true, allow_non_move, buf_size, allocator_type>>
    {
        using this_type = move_box<value_type, allow_non_move, buf_size, allocator_type>;
        using base_type =
            box_functions<_box_impl<value_type, false, true, allow_non_move, buf_size, allocator_type>>;
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
        template <typename type, usize that_buf_size, typename that_allocator_type>
            requires allow_non_move
        constexpr move_box(const copy_box<type, that_buf_size, that_allocator_type>& that)
            : base_type{ typename _impl_type::copy_tag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # template copy operator
        /// ----------------------------------------------------------------------------------------
        template <typename type, usize that_buf_size, typename that_allocator_type>
            requires allow_non_move
        constexpr move_box& operator=(const copy_box<type, that_buf_size, that_allocator_type>& that)
        {
            _impl.move_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # template copy constructor
        /// ----------------------------------------------------------------------------------------
        template <typename type, usize that_buf_size, typename that_allocator_type>
            requires allow_non_move
        constexpr move_box(
            const copy_move_box<type, allow_non_move, that_buf_size, that_allocator_type>& that)
            : base_type{ typename _impl_type::copy_tag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # template copy operator
        /// ----------------------------------------------------------------------------------------
        template <typename type, usize that_buf_size, typename that_allocator_type>
            requires allow_non_move
        constexpr move_box& operator=(
            const copy_move_box<type, allow_non_move, that_buf_size, that_allocator_type>& that)
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
        template <typename type, usize that_buf_size, typename that_allocator_type>
        constexpr move_box(
            copy_move_box<type, allow_non_move, that_buf_size, that_allocator_type>&& that)
            : base_type{ typename _impl_type::move_tag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # template move operator
        /// ----------------------------------------------------------------------------------------
        template <typename type, usize that_buf_size, typename that_allocator_type>
        constexpr move_box& operator=(
            copy_move_box<type, allow_non_move, that_buf_size, that_allocator_type>&& that)
        {
            _impl.move_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # constructor
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr move_box(type&& obj)
            : base_type{ forward<type>(obj) }
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
        using base_type::_impl;
    };

    template <typename value_type, bool allow_non_move, usize buf_size, typename allocator_type>
    class copy_move_box
        : public box_functions<
              _box_impl<value_type, true, true, allow_non_move, buf_size, allocator_type>>
    {
        using this_type = copy_move_box<value_type, allow_non_move, buf_size, allocator_type>;
        using base_type =
            box_functions<_box_impl<value_type, true, true, allow_non_move, buf_size, allocator_type>>;
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
        template <typename type, usize that_buf_size, typename that_allocator_type>
            requires allow_non_move
        constexpr copy_move_box(const copy_box<type, that_buf_size, that_allocator_type>& that)
            : base_type{ typename _impl_type::copy_tag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// memory allocator.
        /// ----------------------------------------------------------------------------------------
        template <typename type, usize that_buf_size, typename that_allocator_type>
            requires allow_non_move
        constexpr copy_move_box& operator=(
            const copy_box<type, that_buf_size, that_allocator_type>& that)
        {
            _impl.copy_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// heap memory allocated.
        /// ----------------------------------------------------------------------------------------
        template <typename type, usize that_buf_size, typename that_allocator_type>
            requires allow_non_move
        constexpr copy_move_box(
            const copy_move_box<type, allow_non_move, that_buf_size, that_allocator_type>& that)
            : base_type{ typename _impl_type::copy_tag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// size of heap memory allocated.
        /// ----------------------------------------------------------------------------------------
        template <typename type, usize that_buf_size, typename that_allocator_type>
            requires allow_non_move
        constexpr copy_move_box& operator=(
            const copy_move_box<type, allow_non_move, that_buf_size, that_allocator_type>& that)
        {
            _impl.copy_box(that._impl);
            return *this;
        }

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
        template <typename type, bool that_allow_non_move, usize that_buf_size,
            typename that_allocator_type>
            requires allow_non_move
        constexpr copy_move_box(
            move_box<type, that_allow_non_move, that_buf_size, that_allocator_type>&& that)
            : base_type{ typename _impl_type::move_tag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # template move operator
        /// ----------------------------------------------------------------------------------------
        template <typename type, bool that_allow_non_move, usize that_buf_size,
            typename that_allocator_type>
            requires allow_non_move
        constexpr copy_move_box& operator=(
            move_box<type, that_allow_non_move, that_buf_size, that_allocator_type>&& that)
        {
            _impl.move_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # template move constructor
        /// ----------------------------------------------------------------------------------------
        template <typename type, bool that_allow_non_move, usize that_buf_size,
            typename that_allocator_type>
            requires allow_non_move
        constexpr copy_move_box(
            copy_move_box<type, that_allow_non_move, that_buf_size, that_allocator_type>&& that)
            : base_type{ typename _impl_type::move_tag(), that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # template move operator
        /// ----------------------------------------------------------------------------------------
        template <typename type, bool that_allow_non_move, usize that_buf_size,
            typename that_allocator_type>
            requires allow_non_move
        constexpr copy_move_box& operator=(
            copy_move_box<type, that_allow_non_move, that_buf_size, that_allocator_type>&& that)
        {
            _impl.move_box(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # constructor
        /// ----------------------------------------------------------------------------------------
        template <typename type>
        constexpr copy_move_box(type&& obj)
            : base_type{ forward<type>(obj) }
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
