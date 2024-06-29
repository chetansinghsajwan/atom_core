export module atom.core:core.option;

import :types;
import :contracts;
import :core.core;
import :core.option_impl;

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// the option class is used to wrap the object of type `value_type`. this_type class contain either the
    /// value or can be empty representing no value.
    ///
    /// this_type is useful when we want to return a value that may or may not exist, without
    /// using null pointers or exceptions. or just want to add the ability of being null to a type
    /// like `i32`.
    ///
    /// # template parameters
    /// - `type`: type of value to store.
    /// --------------------------------------------------------------------------------------------
    export template <typename in_value_type>
    class option
    {
        template <typename that_value_type>
        friend class option;

    private:
        using this_type = option;
        using impl_type = option_impl<in_value_type>;
        using value_type_info = type_info<in_value_type>;

    public:
        static_assert(not value_type_info::is_void(), "value type must not be void.");
        static_assert(value_type_info::is_pure(), "value type must be pure.");
        static_assert(value_type_info::is_destructible(), "value type must be destructible.");

    public:
        /// ----------------------------------------------------------------------------------------
        /// type of value this option holds.
        /// ----------------------------------------------------------------------------------------
        using value_type = in_value_type;

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        ///
        /// constructs with no value.
        /// ----------------------------------------------------------------------------------------
        constexpr option()
            : _impl{ typename impl_type::null_tag{} }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # trivial copy constructor.
        /// ----------------------------------------------------------------------------------------
        constexpr option(const this_type& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # copy constructor
        ///
        /// if `that` contains value, copy constructs `this` value with `that` value.
        /// else constructs wih no value.
        /// ----------------------------------------------------------------------------------------
        constexpr option(const this_type& that)
            requires(value_type_info::is_copy_constructible()
                     and not value_type_info::is_trivially_copy_constructible())
            : _impl{ typename impl_type::that_tag{}, that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # trivial copy operator
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(const this_type& that) -> this_type& = default;

        /// ----------------------------------------------------------------------------------------
        /// # copy operator
        ///
        /// if `that` contains value
        ///     if `this` contains value, copy assigns with `that` value.
        ///     else, copy constructs with `that` value.
        /// else
        ///     if `this` contains value, destroys `this` value.
        ///     else, does nothing.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(const this_type& that) -> this_type&
            requires(value_type_info::is_copyable()
                     and not value_type_info::is_trivially_copy_assignable())
        {
            _impl.set_value_that(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # trivial move constructor
        /// ----------------------------------------------------------------------------------------
        constexpr option(this_type&& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # move constructor
        ///
        /// if `that` contains value, move constructs `this` value with `that` value.
        /// else constructs wih no value.
        /// ----------------------------------------------------------------------------------------
        constexpr option(this_type&& that)
            requires(value_type_info::is_move_constructible()
                     and not value_type_info::is_trivially_move_constructible())
            : _impl{ typename impl_type::that_tag{}, move(that._impl) }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # trivial move operator
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(this_type&& that) -> this_type& = default;

        /// ----------------------------------------------------------------------------------------
        /// # move operator
        ///
        /// if `that` contains value
        ///     if `this` contains value, move assigns with `that` value.
        ///     else, move constructs with `that` value.
        /// else
        ///     if `this` contains value, destroys `this` value.
        ///     else, does nothing.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(this_type&& that) -> this_type&
            requires(value_type_info::is_moveable()
                     and not value_type_info::is_trivially_move_assignable())
        {
            _impl.set_value_that(move(that._impl));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # null constructor
        ///
        /// constructs with no value.
        /// ----------------------------------------------------------------------------------------
        constexpr option(create_from_null_tag)
            : _impl{ typename impl_type::null_tag{} }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # value copy constructor
        ///
        /// copy constructs `this` value with `value`.
        ///
        /// # parameters
        ///
        /// - `value`: value to construct with.
        /// ----------------------------------------------------------------------------------------
        constexpr option(const value_type& value)
            : _impl{ typename impl_type::emplace_tag{}, value }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # value copy operator
        ///
        /// if `this` contains value, copy assigns `this` value with `value`.
        /// else, copy constructs `this` value with `value`.
        ///
        /// # parameters
        ///
        /// - `value`: value to assign or construct with.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(const value_type& value) -> this_type&
        {
            _impl.emplace_value(value);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # value move constructor
        ///
        /// move constructs `this` value with `value`.
        ///
        /// # parameters
        ///
        /// - `value`: value to construct with.
        /// ----------------------------------------------------------------------------------------
        constexpr option(value_type&& value)
            : _impl{ typename impl_type::emplace_tag{}, move(value) }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # value move operator
        ///
        /// if `this` contains value, move assigns `this` value with `value`.
        /// else, move constructs `this` value with `value`.
        ///
        /// # parameters
        ///
        /// - `value`: value to assign or construct with.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator=(value_type&& value) -> this_type&
        {
            _impl.emplace_value(move(value));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # trivial destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~option() = default;

    public:
        /// ----------------------------------------------------------------------------------------
        /// construct value with `args`. if a value already exists, destroys that value and
        /// contructs and new one.
        ///
        /// # parameters
        ///
        /// - `args`: arguments to construct the new value with.
        /// ----------------------------------------------------------------------------------------
        template <typename... arg_types>
        constexpr auto emplace(arg_types&&... args)
            requires(value_type_info::template is_constructible_from<arg_types...>())
        {
            _impl.emplace_value(forward<arg_types>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the stored value.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get() & -> value_type&
        {
            contract_debug_expects(is_value(), "option does not contain value.");

            return _impl.get_value();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the stored value.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get() && -> value_type&&
        {
            contract_debug_expects(is_value(), "option does not contain value.");

            return move(_impl.get_value());
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the stored value.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get() const -> const value_type&
        {
            contract_debug_expects(is_value(), "option does not contain value.");

            return _impl.get_value();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the stored value.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_checked() & -> value_type&
        {
            contract_expects(is_value(), "option does not contain value.");

            return _impl.get_value();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the stored value.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_checked() && -> value_type&&
        {
            contract_expects(is_value(), "option does not contain value.");

            return move(_impl.get_value());
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the stored value.
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_checked() const -> const value_type&
        {
            contract_expects(is_value(), "option does not contain value.");

            return _impl.get_value();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the stored value.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator*() & -> value_type&
        {
            contract_debug_expects(is_value(), "option does not contain value.");

            return _impl.get_value();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the stored value.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator*() && -> value_type&&
        {
            contract_debug_expects(is_value(), "option does not contain value.");

            return move(_impl.get_value());
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the stored value.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator*() const -> const value_type&
        {
            contract_debug_expects(is_value(), "option does not contain value.");

            return _impl.get_value();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the stored value.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator->() -> value_type*
        {
            contract_debug_expects(is_value(), "option does not contain value.");

            return &_impl.get_value();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the stored value.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator->() const -> const value_type*
        {
            contract_debug_expects(is_value(), "option does not contain value.");

            return &_impl.get_value();
        }

        /// ----------------------------------------------------------------------------------------
        /// destroys current value if any.
        /// ----------------------------------------------------------------------------------------
        constexpr auto reset()
        {
            return _impl.reset_value();
        }

        /// ----------------------------------------------------------------------------------------
        /// return `true` if `this` contains value.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_value() const -> bool
        {
            return _impl.is_value();
        }

        /// --------------------------------------------------------------------------------------------
        /// # equality comparision
        ///
        /// if `this` and `that` are null, returns `true`.
        /// if `this` is null and `that` is not null or vice versa, returns `false`.
        /// if `this` and `that` are not null, returns `this.get() == that.get()`.
        /// --------------------------------------------------------------------------------------------
        template <typename that_value_type>
        constexpr auto operator==(const option<that_value_type>& that) const -> bool
            requires(value_type_info::template is_equality_comparable_with<that_value_type>())
        {
            return _impl.is_eq(that._impl);
        }

        /// --------------------------------------------------------------------------------------------
        /// # less than comparision
        ///
        /// if `this` or `that` is null, returns false.
        /// else, returns `this.get() < that.get()`.
        /// --------------------------------------------------------------------------------------------
        template <typename that_value_type>
        constexpr auto operator<(const option<that_value_type>& that) const -> bool
            requires(value_type_info::template is_comparable_with<that_value_type>())
        {
            return _impl.is_lt(that._impl);
        }

        /// --------------------------------------------------------------------------------------------
        /// # greater than comparision
        ///
        /// if `opt0` or `that` is null, returns false.
        /// else, returns `this.get() > that.get()`.
        /// --------------------------------------------------------------------------------------------
        template <typename that_value_type>
        constexpr auto operator>(const option<that_value_type>& that) const -> bool
            requires(value_type_info::template is_comparable_with<that_value_type>())
        {
            return _impl.is_gt(that._impl);
        }

        /// --------------------------------------------------------------------------------------------
        /// # less than or equal to comparision
        ///
        /// if `opt0` or `that` is null, returns false.
        /// else, returns `this.get() <= that.get()`.
        /// --------------------------------------------------------------------------------------------
        template <typename that_value_type>
        constexpr auto operator<=(const option<that_value_type>& that) const -> bool
            requires(value_type_info::template is_comparable_with<that_value_type>())
        {
            return _impl.is_le(that._impl);
        }

        /// --------------------------------------------------------------------------------------------
        /// # greater than or equal to comparision
        ///
        /// if `opt0` or `that` is null, returns false.
        /// else, returns `this.get() >= that.get()`.
        /// --------------------------------------------------------------------------------------------
        template <typename that_value_type>
        constexpr auto operator>=(const option<that_value_type>& that) const -> bool
            requires(value_type_info::template is_comparable_with<that_value_type>())
        {
            return _impl.is_ge(that._impl);
        }

    private:
        impl_type _impl;
    };
}
