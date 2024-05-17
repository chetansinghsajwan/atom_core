#pragma once
// #include "atom/core/_std.h"
#include "atom/core/memory/box.h"
#include "atom/core/core.h"
// #include "atom/core/contracts.h"

/// ------------------------------------------------------------------------------------------------
/// implementations
/// ------------------------------------------------------------------------------------------------
namespace atom
{
    class invokable_box_tag
    {};

    template <typename result_t, typename... arg_ts>
    class _invokable_box_interface
    {
    public:
        virtual auto invoke(arg_ts... args) -> result_t = 0;
    };

    template <typename invokable_t, typename result_t, typename... arg_ts>
    class _invokable_box_wrapper: public _invokable_box_interface<result_t, arg_ts...>
    {
    public:
        _invokable_box_wrapper(invokable_t&& invokable)
            : _invokable(invokable)
        {}

    public:
        virtual auto invoke(arg_ts... args) -> result_t override final
        {
            if constexpr (typeinfo<result_t>::is_void)
            {
                _invokable(forward<arg_ts>(args)...);
            }
            else
            {
                return _invokable(forward<arg_ts>(args)...);
            }
        }

    public:
        invokable_t _invokable;
    };

    /// --------------------------------------------------------------------------------------------
    /// stores an invokable value_t using boxing.
    /// --------------------------------------------------------------------------------------------
    template <typename result_t, typename... arg_ts>
    class _invokable_box_impl
    {
        using this_t = _invokable_box_impl<result_t, arg_ts...>;
        using _box_t = std::function<result_t(arg_ts...)>;
        // using _interface_t = _invokable_box_interface<result_t, arg_ts...>;
        // using _box_t = copy_move_box<_interface_t, true, 50>;

        // template <typename invokable_t>
        // using _wrapper_t = _invokable_box_wrapper<invokable_t, result_t, arg_ts...>;

    public:
        class copy_tag
        {};

        class move_tag
        {};

        class value_tag
        {};

    public:
        _invokable_box_impl()
            : _box()
        {}

        _invokable_box_impl(copy_tag, const this_t& that)
            : _box(that._box)
        {}

        auto copy_that(const this_t& that)
        {
            _box = that._box;
        }

        _invokable_box_impl(move_tag, this_t& that)
            : _box(atom::move(that._box))
        {}

        auto move_that(this_t& that)
        {
            _box = atom::move(that._box);
        }

        template <typename invokable_t>
        _invokable_box_impl(value_tag, invokable_t&& invokable)
            : _box(forward<invokable_t>(invokable))
        {}

        ~_invokable_box_impl() {}

    public:
        /// ----------------------------------------------------------------------------------------
        /// stores invokable.
        /// ----------------------------------------------------------------------------------------
        template <typename invokable_t>
        auto set_invokable(invokable_t&& invokable)
        {
            // _box.set_val(_wrapper_t<invokable_t>(forward<invokable_t>(invokable)));
            _box = forward<invokable_t>(invokable);
        }

        /// ----------------------------------------------------------------------------------------
        /// get invokable.
        /// ----------------------------------------------------------------------------------------
        template <typename invokable_t>
        auto get_invokable_as() -> invokable_t*
        {
            // if (typeid(invokable_t) != get_invokable_t())
            //     return nullptr;

            // return _box.template mut_mem_as<invokable_t>();
            return _box.template get_t<invokable_t>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        auto get_invokable_t() const -> const std::type_info&
        {
            // return _box.val_t();
            return _box.target_t();
        }

        /// ----------------------------------------------------------------------------------------
        /// get invokable.
        /// ----------------------------------------------------------------------------------------
        auto has_invokable() const -> bool
        {
            // return _box.has_val();
            return (bool)_box;
        }

        /// ----------------------------------------------------------------------------------------
        /// invokes stored invokable.
        /// ----------------------------------------------------------------------------------------
        auto invoke_invokable(arg_ts&&... args) -> result_t
        {
            // return _box.get_mut().invoke(forward<arg_ts>(args)...);
            return _box(forward<arg_ts>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// destroys stored invokable if any.
        /// ----------------------------------------------------------------------------------------
        auto destroy_invokable()
        {
            // _box.destroy();
            _box = nullptr;
        }

    private:
        _box_t _box;
    };
}

/// ------------------------------------------------------------------------------------------------
/// apis
/// ------------------------------------------------------------------------------------------------
namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// [`invokable_box`] declaration.
    /// --------------------------------------------------------------------------------------------
    template <typename signature>
    class invokable_box;

    /// --------------------------------------------------------------------------------------------
    /// stores an invokable value_t using boxing.
    /// --------------------------------------------------------------------------------------------
    template <typename result_t, typename... arg_ts>
    class invokable_box<result_t(arg_ts...)>: public invokable_box_tag
    {
        using _impl_t = _invokable_box_impl<result_t, arg_ts...>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor.
        /// ----------------------------------------------------------------------------------------
        constexpr invokable_box()
            : _impl()
        {}

        /// ----------------------------------------------------------------------------------------
        /// # copy constructor
        /// ----------------------------------------------------------------------------------------
        invokable_box(const invokable_box& that)
            : _impl(typename _impl_t::copy_tag(), that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # copy operator
        /// ----------------------------------------------------------------------------------------
        auto operator=(const invokable_box& that) -> invokable_box&
        {
            _impl.copy_that(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # move constructor
        /// ----------------------------------------------------------------------------------------
        invokable_box(invokable_box&& that)
            : _impl(typename _impl_t::move_tag(), that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # move operator
        /// ----------------------------------------------------------------------------------------
        auto operator=(invokable_box&& that) -> invokable_box&
        {
            _impl.move_that(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # null constructor.
        /// ----------------------------------------------------------------------------------------
        invokable_box(nullptr_t null)
            : _impl()
        {}

        /// ----------------------------------------------------------------------------------------
        /// # null operator.
        /// ----------------------------------------------------------------------------------------
        auto operator=(nullptr_t null) -> invokable_box&
        {
            _impl.destroy_invokable();
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # value constructor
        /// ----------------------------------------------------------------------------------------
        template <typename invokable_t>
        invokable_box(invokable_t&& invokable)
            requires(typeinfo<invokable_t>::template is_invokable<result_t(arg_ts...)>)
                    and (typeinfo<invokable_t>::template is_not_derived_from<invokable_box_tag>)
            : _impl(typename _impl_t::value_tag(), forward<invokable_t>(invokable))
        {}

        /// ----------------------------------------------------------------------------------------
        /// # value operator
        /// ----------------------------------------------------------------------------------------
        template <typename invokable_t>
        invokable_box& operator=(invokable_t&& invokable)
            requires typeinfo<invokable_t>::template
        is_invokable<result_t(arg_ts...)>
            and (typeinfo<invokable_t>::template is_not_derived_from<invokable_box_tag>)
        {
            _impl.set_invokable(forward<invokable_t>(invokable));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # destructor
        /// ----------------------------------------------------------------------------------------
        ~invokable_box() {}

    public:
        /// ----------------------------------------------------------------------------------------
        /// stores the invokable.
        /// ----------------------------------------------------------------------------------------
        template <typename invokable_t>
        auto set(invokable_t&& invokable)
        {
            _impl.set_invokable(forward<invokable_t>(invokable));
        }

        /// ----------------------------------------------------------------------------------------
        /// returns stored invokable as a `mut_ptr` to `value_t`.
        /// ----------------------------------------------------------------------------------------
        template <typename value_t>
        auto get_as() -> value_t*
        {
            return _impl.template get_invokable_as<value_t>();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the typeid for the stored invokable.
        /// ----------------------------------------------------------------------------------------
        auto get_t() const -> const std::type_info&
        {
            return _impl.get_invokable_t();
        }

        /// ----------------------------------------------------------------------------------------
        /// invokes the stored invokable.
        /// ----------------------------------------------------------------------------------------
        auto invoke(arg_ts&&... args) -> result_t
        {
            contract_expects(has(), "no invokable is present.");

            return _impl.invoke_invokable(forward<arg_ts>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// invokes the stored invokable if any.
        /// ----------------------------------------------------------------------------------------
        auto invoke_try(result_t* out, arg_ts&&... args) -> result_t
        {
            if (not _impl.has_invokable())
                return;

            *out = _impl.invoke_invokable(forward<arg_ts>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `invoke(args...)`.
        /// ----------------------------------------------------------------------------------------
        auto operator()(arg_ts&&... args) -> result_t
        {
            return invoke(forward<arg_ts>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if this doesn't contain an invokable.
        /// ----------------------------------------------------------------------------------------
        auto has() const -> bool
        {
            return _impl.has_invokable();
        }

        /// ----------------------------------------------------------------------------------------
        /// destroys stored invokable if any.
        /// ----------------------------------------------------------------------------------------
        auto destroy()
        {
            return _impl.destroy_invokable();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if this doesn't contain an invokable.
        /// ----------------------------------------------------------------------------------------
        auto is_eq(nullptr_t null) const -> bool
        {
            return not _impl.has_invokable();
        }

    private:
        _impl_t _impl;
    };
}
