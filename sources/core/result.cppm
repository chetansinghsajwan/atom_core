export module atom.core:core.result;

import :types;
import :contracts;
import :core.core;
import :core.nums;
import :core.option;
import :core.result_impl;

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// `result` template type is derived from `this` type, this helps to identify if a type is
    /// `result` template type.
    /// --------------------------------------------------------------------------------------------
    export class result_tag
    {};

    /// --------------------------------------------------------------------------------------------
    /// this type is used to represent the result of an operation. it can either store the value of
    /// `value_type` or an error of one of the `error_types`.
    /// --------------------------------------------------------------------------------------------
    export template <typename in_value_type, typename... error_types>
    class result: public result_tag
    {
        template <typename that_value_type, typename... that_error_types>
        friend class result;

    private:
        using this_type = result;
        using impl_type = result_impl<in_value_type, error_types...>;
        using value_type_info = type_info<in_value_type>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// type of value `this` supports.
        /// ----------------------------------------------------------------------------------------
        using value_type = in_value_type;

        /// ----------------------------------------------------------------------------------------
        /// `type_list` of error types `this` supports.
        /// ----------------------------------------------------------------------------------------
        using error_types_list = typename impl_type::error_types_list;

    public:
        static_assert(value_type_info::is_pure(), "value type must be pure.");
        static_assert(value_type_info::is_destructible() or value_type_info::is_void(),
            "value type must be destructible");
        static_assert(error_types_list::get_count() > 0, "there must be at least 1 error type.");
        static_assert(error_types_list::are_unique(), "each error type must be unique.");
        static_assert(error_types_list::are_pure(), "each error type must be pure.");
        static_assert(
            error_types_list::are_destructible(), "each error type must be destructible.");

    public:
        /// ----------------------------------------------------------------------------------------
        /// -- include definition.
        /// ----------------------------------------------------------------------------------------
        static consteval auto is_copy_constructor_enabled() -> bool
        {
            if (not value_type_info::is_copy_constructible())
                return false;

            if (not error_types_list::are_copy_constructible())
                return false;

            if (value_type_info::is_copy_constructible()
                and error_types_list::are_trivially_copy_constructible())
                return false;

            return true;
        };

        /// ----------------------------------------------------------------------------------------
        /// -- include definition.
        ///
        /// accepts other result types whose value type is convertible to this value type and
        /// whose error types are a subset of this type.
        /// ----------------------------------------------------------------------------------------
        template <typename that_unpure_type>
        static consteval auto is_universal_copy_constructor_enabled() -> bool
        {
            using that_type = type_info<that_unpure_type>::pure_type::value_type;

            if constexpr (not type_info<that_type>::template is_derived_from<result_tag>())
                return false;

            else if (not value_type_info::template is_constructible_from<
                         typename that_type::value_type>())
                return false;

            else if (not error_types_list::has_all(typename that_type::error_types_list()))
                return false;

            else if (not that_type::error_types_list::are_copy_constructible())
                return false;

            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// -- include definition.
        /// ----------------------------------------------------------------------------------------
        static consteval auto is_copy_operator_enabled() -> bool
        {
            if (not value_type_info::is_copyable())
                return false;

            if (not error_types_list::are_copyable())
                return false;

            if (value_type_info::is_trivially_copyable()
                and error_types_list::are_trivially_copyable())
                return false;

            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// -- include definition.
        ///
        /// accepts other result types whose value type is convertible to this value type and
        /// whose error types are a subset of this type.
        /// ----------------------------------------------------------------------------------------
        template <typename that_unpure_type>
        static consteval auto is_universal_copy_operator_enabled() -> bool
        {
            using that_type = type_info<that_unpure_type>::pure_type::value_type;

            if constexpr (not type_info<that_type>::template is_derived_from<result_tag>())
                return false;

            else if (not value_type_info::template is_constructible_from<
                         typename that_type::value_type>()
                     or not value_type_info::template is_assignable_from<
                         typename that_type::value_type>())
                return false;

            else if (not error_types_list::has_all(typename that_type::error_types_list()))
                return false;

            else if (not error_types_list::are_copyable())
                return false;

            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// -- include definition.
        /// ----------------------------------------------------------------------------------------
        static consteval auto is_move_constructor_enabled() -> bool
        {
            if (not value_type_info::is_move_constructible())
                return false;

            if (not error_types_list::are_move_constructible())
                return false;

            if (value_type_info::is_trivially_move_constructible()
                and error_types_list::are_trivially_move_constructible())
                return false;

            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// -- include definition.
        ///
        /// accepts other result types whose value type is convertible to this value type and
        /// whose error types are a subset of this type.
        /// ----------------------------------------------------------------------------------------
        template <typename that_unpure_type>
        static consteval auto is_universal_move_constructor_enabled() -> bool
        {
            using that_type = type_info<that_unpure_type>::pure_type::value_type;

            if constexpr (not type_info<that_type>::template is_derived_from<result_tag>())
                return false;

            else if (not value_type_info::template is_constructible_from<
                         typename that_type::value_type>())
                return false;

            else if (not error_types_list::has_all(typename that_type::error_types_list()))
                return false;

            else if (not that_type::error_types_list::are_move_constructible())
                return false;

            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// -- include definition.
        /// ----------------------------------------------------------------------------------------
        static consteval auto is_move_operator_enabled() -> bool
        {
            if (not value_type_info::is_moveable())
                return false;

            if (not error_types_list::are_moveable())
                return false;

            if (value_type_info::is_trivially_moveable()
                and error_types_list::are_trivially_moveable())
                return false;

            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// -- include definition.
        ///
        /// accepts other result types whose value type is convertible to this value type and
        /// whose error types are a subset of this type.
        /// ----------------------------------------------------------------------------------------
        template <typename that_unpure_type>
        static consteval auto is_universal_move_operator_enabled() -> bool
        {
            using that_type = type_info<that_unpure_type>::pure_type::value_type;

            if constexpr (not type_info<that_type>::template is_derived_from<result_tag>())
                return false;

            else if (not value_type_info::template is_constructible_from<
                         typename that_type::value_type>()
                     or not value_type_info::template is_assignable_from<
                         typename that_type::value_type>())
                return false;

            else if (not error_types_list::has_all(typename that_type::error_types_list()))
                return false;

            else if (not that_type::error_types_list::are_moveable())
                return false;

            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// -- include definition.
        /// ----------------------------------------------------------------------------------------
        template <typename that_type>
        static consteval auto is_result_constructor_enabled() -> bool
        {
            if constexpr (not type_info<that_type>::template is_derived_from<result_tag>())
                return false;

            else if (type_info<this_type>::template is_same_as<that_type>())
                return false;

            else if (type_info<value_type>::template is_same_as<typename that_type::value_type>())
                return true;

            else if (error_types_list::has_any(typename that_type::error_types_list{}))
                return true;

            return false;
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor
        /// ----------------------------------------------------------------------------------------
        constexpr result() = delete;

        /// ----------------------------------------------------------------------------------------
        /// # trivial copy constructor
        /// ----------------------------------------------------------------------------------------
        constexpr result(const this_type& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # copy constructor
        /// ----------------------------------------------------------------------------------------
        constexpr result(const this_type& that)
            requires(is_copy_constructor_enabled())
            : _impl{ typename impl_type::that_tag{}, that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # universal copy constructor
        /// ----------------------------------------------------------------------------------------
        template <typename that_type>
        constexpr result(const that_type& that)
            requires(is_universal_copy_constructor_enabled<that_type>())
            : _impl{ typename impl_type::that_tag{}, that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # trivial copy operator
        /// ----------------------------------------------------------------------------------------
        constexpr this_type& operator=(const this_type& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # copy operator
        /// ----------------------------------------------------------------------------------------
        constexpr this_type& operator=(const this_type& that)
            requires(is_copy_operator_enabled())
        {
            _impl.set_that(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # universal copy operator
        /// ----------------------------------------------------------------------------------------
        template <typename that_type>
        constexpr this_type& operator=(const that_type& that)
            requires(is_universal_copy_operator_enabled<that_type>())
        {
            _impl.set_that(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # trivial move constructor
        /// ----------------------------------------------------------------------------------------
        constexpr result(this_type&& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # move constructor
        /// ----------------------------------------------------------------------------------------
        constexpr result(this_type&& that)
            requires(is_move_constructor_enabled())
            : _impl{ typename impl_type::that_tag{}, that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # universal move constructor
        /// ----------------------------------------------------------------------------------------
        template <typename that_type>
        constexpr result(that_type&& that)
            requires(is_universal_move_constructor_enabled<that_type>())
            : _impl{ typename impl_type::that_tag{}, that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # trivial move operator
        /// ----------------------------------------------------------------------------------------
        constexpr this_type& operator=(this_type&& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # move operator
        /// ----------------------------------------------------------------------------------------
        constexpr this_type& operator=(this_type&& that)
            requires(is_move_operator_enabled())
        {
            _impl.set_that(move(that._impl));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # universal move operator
        /// ----------------------------------------------------------------------------------------
        template <typename that_type>
        constexpr this_type& operator=(that_type&& that)
            requires(is_universal_move_operator_enabled<that_type>())
        {
            _impl.set_that(move(that._impl));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # void value constructor
        ///
        /// if value type is void, constructs with nothing and sets the value flag to `true`.
        /// ----------------------------------------------------------------------------------------
        constexpr result(create_from_void_tag)
            requires(value_type_info::is_void())
            : _impl{ typename impl_type::template emplace_tag<void>{} }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # value constructor
        ///
        /// constructs `this` with `value`.
        /// ----------------------------------------------------------------------------------------
        template <typename this_type = this_type>
        constexpr result(const this_type::value_type& value)
            requires(not value_type_info::is_void())
            : _impl{ typename impl_type::template emplace_tag<value_type>{}, value }
        {}

        template <typename this_type = this_type>
        constexpr result(this_type::value_type&& value)
            requires(not value_type_info::is_void())
            : _impl{ typename impl_type::template emplace_tag<value_type>{}, move(value) }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # value operator
        ///
        /// constructs or assigns the new value.
        /// ----------------------------------------------------------------------------------------
        template <typename this_type = this_type>
        constexpr this_type& operator=(const this_type::value_type& value)
            requires(not value_type_info::is_void())
        {
            _impl.set_value(value);
            return *this;
        }

        template <typename this_type = this_type>
        constexpr this_type& operator=(this_type::value_type&& value)
            requires(not value_type_info::is_void())
        {
            _impl.set_value(move(value));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # error constructor
        ///
        /// constructs `this` with error of type `error_type`.
        /// ----------------------------------------------------------------------------------------
        template <typename error_type>
        constexpr result(const error_type& error)
            requires(error_types_list::template has<error_type>())
            : _impl{ typename impl_type::template emplace_tag<error_type>{}, error }
        {}

        template <typename error_type>
        constexpr result(error_type&& error)
            requires(error_types_list::template has<error_type>())
            : _impl{ typename impl_type::template emplace_tag<error_type>{}, move(error) }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # error operator
        ///
        /// constructs or assigns error of type `error_type`.
        /// ----------------------------------------------------------------------------------------
        template <typename error_type>
        constexpr this_type& operator=(const error_type& error)
            requires(error_types_list::template has<error_type>())
        {
            _impl.set_error(error);
            return *this;
        }

        template <typename error_type>
        constexpr this_type& operator=(error_type&& error)
            requires(error_types_list::template has<error_type>())
        {
            _impl.set_error(move(error));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # named constructor
        ///
        /// this constructor is used when this doesn't support `value_type` or all of the
        /// `error_types` of `that` but some of them. if the value or error is of some type that
        /// `this` supports, it constructs `this` with that value, else panics.
        /// ----------------------------------------------------------------------------------------
        template <typename that_type>
        constexpr result(create_from_result_tag, const that_type& that)
            requires(is_result_constructor_enabled<that_type>())
            : _impl{ typename impl_type::that_tag{}, that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// -- use above doc
        /// ----------------------------------------------------------------------------------------
        template <typename that_type>
        constexpr result(create_from_result_tag, that_type&& that)
            requires(is_result_constructor_enabled<that_type>())
            : _impl{ typename impl_type::that_tag{}, move(that._impl) }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # named constructor
        ///
        /// constructs `this` by emplacing value or error of type `value_type` with `args`.
        /// ----------------------------------------------------------------------------------------
        template <typename value_type, typename... arg_types>
        constexpr result(create_by_emplace_tag<value_type>, arg_types&&... args)
            requires(is_emplace_constructor_enabled<value_type, arg_types...>())
            : _impl{ typename impl_type::template emplace_tag<value_type>{},
                forward<arg_types>(args)... }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # trivial destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~result() = default;

    public:
        /// ----------------------------------------------------------------------------------------
        /// destroys stored value or error and constructs new value with `args`.
        /// ----------------------------------------------------------------------------------------
        template <typename... arg_types>
        constexpr auto emplace_value(arg_types&&... args) -> void
        {
            _impl.emplace_value(forward<arg_types>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// if value is stored then assign `value` to it, else destroys the error and constructs
        /// value with `value`.
        /// ----------------------------------------------------------------------------------------
        template <typename this_type = this_type>
        constexpr auto set_value(const this_type::value_type& value) -> void
            requires(not value_type_info::is_void())
        {
            _impl.set_value(value);
        }

        template <typename this_type = this_type>
        constexpr auto set_value(this_type::value_type&& value) -> void
            requires(not value_type_info::is_void())
        {
            _impl.set_value(move(value));
        }

        /// ----------------------------------------------------------------------------------------
        /// if `value_type` is `void`, destroys stored error if any, and sets value flag to `true`.
        /// ----------------------------------------------------------------------------------------
        constexpr auto set_value() -> void
            requires(value_type_info::is_void())
        {
            _impl.set_value_void();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the stored value.
        /// ----------------------------------------------------------------------------------------
        template <typename this_type = this_type>
        constexpr auto get_value() & -> this_type::value_type&
            requires(not value_type_info::is_void())
        {
            contract_debug_expects(is_value(), "result is not value.");

            return _impl.get_value();
        }

        template <typename this_type = this_type>
        constexpr auto get_value() && -> this_type::value_type&&
            requires(not value_type_info::is_void())
        {
            contract_debug_expects(is_value(), "result is not value.");

            return move(_impl.get_value());
        }

        template <typename this_type = this_type>
        constexpr auto get_value() const -> const this_type::value_type&
            requires(not value_type_info::is_void())
        {
            contract_debug_expects(is_value(), "result is not value.");

            return _impl.get_value();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the stored value.
        /// ----------------------------------------------------------------------------------------
        template <typename this_type = this_type>
        constexpr auto get_value_checked() & -> this_type::value_type&
            requires(not value_type_info::is_void())
        {
            contract_expects(is_value(), "result is not value.");

            return _impl.get_value();
        }

        template <typename this_type = this_type>
        constexpr auto get_value_checked() && -> this_type::value_type&&
            requires(not value_type_info::is_void())
        {
            contract_expects(is_value(), "result is not value.");

            return move(_impl.get_value());
        }

        template <typename this_type = this_type>
        constexpr auto get_value_checked() const -> const this_type::value_type&
            requires(not value_type_info::is_void())
        {
            contract_expects(is_value(), "result is not value.");

            return _impl.get_value();
        }

        /// ----------------------------------------------------------------------------------------
        ///returns the stored value.
        /// ----------------------------------------------------------------------------------------
        template <typename this_type = this_type>
        constexpr auto get_value_unchecked() & -> this_type::value_type&
            requires(not value_type_info::is_void())
        {
            return _impl.get_value();
        }

        template <typename this_type = this_type>
        constexpr auto get_value_unchecked() && -> this_type::value_type&&
            requires(not value_type_info::is_void())
        {
            return move(_impl.get_value());
        }

        template <typename this_type = this_type>
        constexpr auto get_value_unchecked() const -> const this_type::value_type&
            requires(not value_type_info::is_void())
        {
            return _impl.get_value();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if value is stored.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_value() const -> bool
        {
            return _impl.is_value();
        }

        /// ----------------------------------------------------------------------------------------
        /// destorys current value or error, and constructs new error of `error_type` with `args`.
        /// ----------------------------------------------------------------------------------------
        template <typename error_type, typename... arg_types>
        constexpr auto emplace_error(arg_types&&... args) -> void
            requires(error_types_list::template has<error_type>())
        {
            _impl.template emplace_error<error_type>(forward<arg_types>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// if error of type `error_type` is stored, assigns `error` to it, else destroys current
        /// value or error and constructs a new error with `error`.
        /// ----------------------------------------------------------------------------------------
        template <typename error_type>
        constexpr auto set_error(const error_type& error) -> void
            requires(error_types_list::template has<error_type>())
        {
            _impl.set_error(error);
        }

        template <typename error_type>
        constexpr auto set_error(error_type&& error) -> void
            requires(error_types_list::template has<error_type>())
        {
            _impl.set_error(move(error));
        }

        /// ----------------------------------------------------------------------------------------
        /// returns error of as `error_type`.
        /// ----------------------------------------------------------------------------------------
        template <typename error_type>
        constexpr auto get_error() & -> error_type&
            requires(error_types_list::template has<error_type>())
        {
            contract_debug_expects(is_error<error_type>(), "result is not of requested error type");

            return _impl.template get_error<error_type>();
        }

        template <typename error_type>
        constexpr auto get_error() && -> error_type&&
            requires(error_types_list::template has<error_type>())
        {
            contract_debug_expects(is_error<error_type>(), "result is not of requested error type");

            return move(_impl.template get_error<error_type>());
        }

        template <typename error_type>
        constexpr auto get_error() const -> const error_type&
            requires(error_types_list::template has<error_type>())
        {
            contract_debug_expects(is_error<error_type>(), "result is not of requested error type");

            return _impl.template get_error<error_type>();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns error of as `error_type`.
        /// ----------------------------------------------------------------------------------------
        template <typename error_type>
        constexpr auto get_error_checked() & -> error_type&
            requires(error_types_list::template has<error_type>())
        {
            contract_expects(is_error<error_type>(), "result is not of requested error type.");

            return _impl.template get_error<error_type>();
        }

        template <typename error_type>
        constexpr auto get_error_checked() && -> error_type&&
            requires(error_types_list::template has<error_type>())
        {
            contract_expects(is_error<error_type>(), "result is not of requested error type.");

            return move(_impl.template get_error<error_type>());
        }

        template <typename error_type>
        constexpr auto get_error_checked() const -> const error_type&
            requires(error_types_list::template has<error_type>())
        {
            contract_expects(is_error<error_type>(), "result is not of requested error type.");

            return _impl.template get_error<error_type>();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns error of as `error_type`.
        /// ----------------------------------------------------------------------------------------
        template <typename error_type>
        constexpr auto get_error_unchecked() & -> error_type&
            requires(error_types_list::template has<error_type>())
        {
            return _impl.template get_error<error_type>();
        }

        template <typename error_type>
        constexpr auto get_error_unchecked() && -> error_type&&
            requires(error_types_list::template has<error_type>())
        {
            return move(_impl.template get_error<error_type>());
        }

        template <typename error_type>
        constexpr auto get_error_unchecked() const -> const error_type&
            requires(error_types_list::template has<error_type>())
        {
            return _impl.template get_error<error_type>();
        }

        /// ----------------------------------------------------------------------------------------
        /// return `true` if error of any type is stored.
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_error() const -> bool
        {
            return _impl.is_error();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if error of `error_type` is stored.
        /// ----------------------------------------------------------------------------------------
        template <typename error_type>
        constexpr auto is_error() const -> bool
            requires(error_types_list::template has<error_type>())
        {
            return _impl.template is_error<error_type>();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if error of any of `other_error_types` is stored.
        /// ----------------------------------------------------------------------------------------
        template <typename... other_error_types>
        constexpr auto is_error_any() const -> bool
            requires(error_types_list::template has_all<other_error_types...>())
        {
            return _impl.template is_error_any<other_error_types...>();
        }

        /// ----------------------------------------------------------------------------------------
        /// returns an `option` with stored value. if no value was stored, return null option.
        /// ----------------------------------------------------------------------------------------
        constexpr auto to_option() -> option<value_type>
        {
            return _impl.to_option();
        }

        /// ----------------------------------------------------------------------------------------
        /// return an `option` with stored error of type `error_type`. if error of that type wasn't
        /// stored return null option.
        /// ----------------------------------------------------------------------------------------
        template <typename error_type>
        constexpr auto to_option_error() -> option<error_type>
            requires(error_types_list::template has<error_type>())
        {
            return _impl.template to_option_error<error_type>();
        }

        /// ----------------------------------------------------------------------------------------
        /// -- include definition.
        ///
        /// returns `true` if equality comparition function is present.
        /// ----------------------------------------------------------------------------------------
        template <typename that_type>
        static consteval auto is_equality_conversion_enabled() -> bool
        {
            if constexpr (not type_info<that_type>::template is_derived_from<result_tag>())
                return false;

            else if (not type_info<value_type>::is_equality_comparable())
                return false;

            else if (not error_types_list::are_equality_comparable())
                return false;

            else if (type_info<value_type>::template is_same_as<typename that_type::value_type>())
                return true;

            else if (error_types_list::has_any(typename that_type::error_types_list{}))
                return true;

            return false;
        }

        /// ----------------------------------------------------------------------------------------
        /// # equality comparision operator
        ///
        /// returns `true` if `that` holds the value or same error type as `this` does and then
        /// compares those value or error.
        /// ----------------------------------------------------------------------------------------
        template <typename that_type>
        constexpr auto operator==(const that_type& that) const -> bool
            requires(is_equality_conversion_enabled<that_type>())
        {
            return _impl.is_eq(that._impl);
        }

    private:
        impl_type _impl;
    };
}
