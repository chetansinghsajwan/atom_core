export module atom.core:core.result;

import :types;
import :core.core;
import :core.nums;
import :core.option;

namespace atom
{
    export class result_void
    {};

    export class result_tag
    {};

    /// --------------------------------------------------------------------------------------------
    /// @todo update docs.
    /// --------------------------------------------------------------------------------------------
    export template <typename in_impl_type>
    class result: public result_tag
    {
        template <typename that_impl_type>
        friend class result;

    private:
        using this_type = result;
        using impl_type = in_impl_type;

    public:
        using value_type = typename impl_type::out_value_type;
        using value_type_info = type_info<value_type>;
        using error_types_list = typename impl_type::error_types_list;

    public:
        static_assert(value_type_info::is_pure(), "unpure value type are not supported.");
        static_assert(value_type_info::is_void() or value_type_info::is_destructible(),
            "non destructible value type is not supported.");
        static_assert(error_types_list::get_count() > 0, "there must be at least 1 error type.");
        static_assert(error_types_list::are_unique(), "each error type must be unique.");
        static_assert(error_types_list::are_pure(), "unpure error types are not supported.");
        static_assert(error_types_list::are_destructible(),
            "non destructible error types are not supported.");

    public:
        static consteval auto should_enable_copy_constructor() -> bool
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

        template <typename in_that_type>
        static consteval auto should_enable_universal_copy_constructor() -> bool
        {
            using that_type = type_info<in_that_type>::pure_type::value_type;

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

        static consteval auto should_enable_copy_operator() -> bool
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

        template <typename in_that_type>
        static consteval auto should_enable_universal_copy_operator() -> bool
        {
            using that_type = type_info<in_that_type>::pure_type::value_type;

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

        static consteval auto should_enable_move_constructor() -> bool
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

        template <typename in_that_type>
        static consteval auto should_enable_universal_move_constructor() -> bool
        {
            using that_type = type_info<in_that_type>::pure_type::value_type;

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

        static consteval auto should_enable_move_operator() -> bool
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

        template <typename in_that_type>
        static consteval auto should_enable_universal_move_operator() -> bool
        {
            using that_type = type_info<in_that_type>::pure_type::value_type;

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

        template <typename that_type>
        static consteval auto should_enable_result_constructor() -> bool
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

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename error_type>
        static consteval auto has_error() -> bool
        {
            return impl_type::template has_error<error_type>();
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// # default constructor (deleted)
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
            requires(should_enable_copy_constructor())
            : _impl(typename impl_type::copy_tag{}, that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # universal copy constructor
        ///
        /// accepts other result types whose value type is convertible to this value type and
        /// whose error types are a subset of this type.
        /// ----------------------------------------------------------------------------------------
        template <typename that_type>
        constexpr result(const that_type& that)
            requires(should_enable_universal_copy_constructor<that_type>())
            : _impl(typename impl_type::copy_tag{}, that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # trivial copy operator
        /// ----------------------------------------------------------------------------------------
        constexpr this_type& operator=(const this_type& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # copy operator
        /// ----------------------------------------------------------------------------------------
        constexpr this_type& operator=(const this_type& that)
            requires(should_enable_copy_operator())
        {
            _impl.set_that(that._impl);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # universal copy operator
        /// ----------------------------------------------------------------------------------------
        template <typename that_type>
        constexpr this_type& operator=(const that_type& that)
            requires(should_enable_universal_copy_operator<that_type>())
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
            requires(should_enable_move_constructor())
            : _impl(typename impl_type::move_tag{}, that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # universal move constructor
        /// ----------------------------------------------------------------------------------------
        template <typename that_type>
        constexpr result(that_type&& that)
            requires(should_enable_universal_move_constructor<that_type>())
            : _impl(typename impl_type::move_tag{}, that._impl)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # trivial move operator
        /// ----------------------------------------------------------------------------------------
        constexpr this_type& operator=(this_type&& that) = default;

        /// ----------------------------------------------------------------------------------------
        /// # move operator
        /// ----------------------------------------------------------------------------------------
        constexpr this_type& operator=(this_type&& that)
            requires(should_enable_move_operator())
        {
            _impl.set_that(move(that._impl));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # universal move operator
        /// ----------------------------------------------------------------------------------------
        template <typename that_type>
        constexpr this_type& operator=(that_type&& that)
            requires(should_enable_universal_move_operator<that_type>())
        {
            _impl.set_that(move(that._impl));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # void value constructor
        /// ----------------------------------------------------------------------------------------
        constexpr result(result_void)
            requires(value_type_info::is_void())
            : _impl(typename impl_type::value_tag{}, {})
        {}

        /// ----------------------------------------------------------------------------------------
        /// # value copy constructor
        ///
        /// copy constructs result with value of type type.
        ///
        /// # parameters
        ///
        ///  - `value`: value to construct with.
        /// ----------------------------------------------------------------------------------------
        template <typename this_type = this_type>
        constexpr result(const this_type::value_type& value)
            requires(not value_type_info::is_void())
            : _impl(typename impl_type::value_tag{}, value)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # value move constructor
        ///
        /// move constructs result with value of type type.
        ///
        /// # parameters
        ///
        ///  - `value`: value to construct with.
        /// ----------------------------------------------------------------------------------------
        template <typename this_type = this_type>
        constexpr result(this_type::value_type&& value)
            requires(not value_type_info::is_void())
            : _impl(typename impl_type::value_tag{}, move(value))
        {}

        /// ----------------------------------------------------------------------------------------
        /// # value copy operator
        ///
        /// destroys previous value sets new value.
        ///
        /// # parameters
        ///
        ///  - `value`: value to set.
        /// ----------------------------------------------------------------------------------------
        template <typename this_type = this_type>
        constexpr this_type& operator=(const this_type::value_type& value)
            requires(not value_type_info::is_void())
        {
            _impl.set_value(value);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # value move operator
        ///
        /// destroys previous value sets new value.
        ///
        /// # parameters
        ///
        ///  - `value`: value to set.
        /// ----------------------------------------------------------------------------------------
        template <typename this_type = this_type>
        constexpr this_type& operator=(this_type::value_type&& value)
            requires(not value_type_info::is_void())
        {
            _impl.set_value(move(value));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # error copy constructor
        /// ----------------------------------------------------------------------------------------
        template <typename error_type>
        constexpr result(const error_type& error)
            requires(has_error<typename type_info<error_type>::pure_type::value_type>())
            : _impl(typename impl_type::error_tag{}, error)
        {}

        /// ----------------------------------------------------------------------------------------
        /// # error move constructor
        /// ----------------------------------------------------------------------------------------
        template <typename error_type>
        constexpr result(error_type&& error)
            requires(has_error<typename type_info<error_type>::pure_type::value_type>())
            : _impl(typename impl_type::error_tag{}, move(error))
        {}

        /// ----------------------------------------------------------------------------------------
        /// # error copy operator
        /// ----------------------------------------------------------------------------------------
        template <typename error_type>
        constexpr this_type& operator=(const error_type& error)
            requires(has_error<typename type_info<error_type>::pure_type::value_type>())
        {
            _impl.set_error(error);
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # error move operator
        /// ----------------------------------------------------------------------------------------
        template <typename error_type>
        constexpr this_type& operator=(error_type&& error)
            requires(has_error<typename type_info<error_type>::pure_type::value_type>())
        {
            _impl.set_error(move(error));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        /// # named constructor
        ///
        /// # todo
        ///
        /// - implement this.
        /// ----------------------------------------------------------------------------------------
        template <typename that_type>
        constexpr result(create_from_result_tag, const that_type& that)
            requires(should_enable_result_constructor<that_type>())
            : _impl{ create_from_result, that._impl }
        {}

        /// ----------------------------------------------------------------------------------------
        /// -- use above doc
        /// ----------------------------------------------------------------------------------------
        template <typename that_type>
        constexpr result(create_from_result_tag, that_type&& that)
            requires(should_enable_result_constructor<that_type>())
            : _impl{ create_from_result, move(that._impl) }
        {}

        /// ----------------------------------------------------------------------------------------
        /// # trivial destructor
        /// ----------------------------------------------------------------------------------------
        constexpr ~result() = default;

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename... args_type>
        constexpr auto emplace_value(args_type&&... args) -> void
        {
            _impl.emplace_value(forward<args_type>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename this_type = this_type>
        constexpr auto set_value(const this_type::value_type& value) -> void
            requires(not value_type_info::is_void())
        {
            _impl.set_value(value);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename this_type = this_type>
        constexpr auto set_value(this_type::value_type&& value) -> void
            requires(not value_type_info::is_void())
        {
            _impl.set_value(move(value));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto set_value() -> void
            requires value_type_info::is_void
        {
            _impl.set_value_void();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename this_type = this_type>
        constexpr auto get_value() const -> const this_type::value_type&
            requires(not value_type_info::is_void())
        {
            return _impl.get_value();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename this_type = this_type>
        constexpr auto get_value() & -> this_type::value_type&
            requires(not value_type_info::is_void())
        {
            return _impl.get_value();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename this_type = this_type>
        constexpr auto get_value() && -> this_type::value_type&&
            requires(not value_type_info::is_void())
        {
            return move(_impl.get_value());
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename this_type = this_type>
        constexpr auto get_value_checked() -> this_type::value_type&
            requires(not value_type_info::is_void())
        {
            return _impl.get_value_checked();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename this_type = this_type>
        constexpr auto get_value_checked() const -> const this_type::value_type&
            requires(not value_type_info::is_void())
        {
            return _impl.get_value_checked();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_value() const -> bool
        {
            return _impl.is_value();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto get_error_count() const -> usize
        {
            return _impl.get_error_count();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename error_type, typename... args_type>
        constexpr auto emplace_error(args_type&&... args) -> void
            requires(has_error<error_type>())
        {
            _impl.template emplace_error<error_type>(forward<args_type>(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename error_type>
        constexpr auto set_error(const error_type& error) -> void
            requires(has_error<error_type>())
        {
            _impl.set_error(error);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename error_type>
        constexpr auto set_error(error_type&& error) -> void
            requires(has_error<error_type>())
        {
            _impl.set_error(move(error));
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename error_type>
        constexpr auto get_error() const -> const error_type&
            requires(has_error<error_type>())
        {
            return _impl.template get_error<error_type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename error_type>
        constexpr auto get_error() -> error_type&
            requires(has_error<error_type>())
        {
            return _impl.template get_error<error_type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto is_error() const -> bool
        {
            return _impl.is_error();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename error_type>
        constexpr auto is_error() const -> bool
            requires(has_error<error_type>())
        {
            return _impl.template is_error<error_type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename... other_error_types>
        constexpr auto is_error_any() const -> bool
            requires(error_types_list::template has_all<other_error_types...>())
        {
            return _impl.template is_error_any<other_error_types...>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto panic_on_error() const -> void
        {
            _impl.panic_on_error();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto to_option() -> option<value_type>
        {
            return _impl.to_option();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename error_type>
        constexpr auto to_option_error() -> option<error_type>
            requires(has_error<error_type>())
        {
            return _impl.template to_option_error<error_type>();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename final_qualified_type, typename function_qualified_type>
        static consteval auto should_enable_on_value_function() -> bool
        {
            using this_type = type_info<final_qualified_type>::pure_type::value_type;

            if constexpr (type_info<this_type>::template is_derived_from<result_tag>())
            {
                using value_qualified_type =
                    type_info<typename this_type::value_type>::template unpure_like_type<
                        final_qualified_type>::value_type;
                using signature_type = void(value_qualified_type);

                if (not type_info<function_qualified_type>::template is_function<signature_type>)
                    return false;

                return true;
            }

            return false;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename function_type>
        constexpr auto on_value(function_type&& action) -> this_type
            requires(should_enable_on_value_function<this_type, function_type>())
        {
            _impl.on_value(forward<function_type>(action));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename final_qualified_type, typename function_qualified_type,
            typename error_type>
        static consteval auto should_enable_on_error_function() -> bool
        {
            using this_type = type_info<final_qualified_type>::pure_type::value_type;

            if constexpr (type_info<this_type>::template is_derived_from<result_tag>())
            {
                if (not type_info<error_type>::is_pure)
                    return false;

                if (not has_error<error_type>())
                    return false;

                using error_qualified_type = type_info<error_type>::template unpure_like_type<
                    final_qualified_type>::value_type;
                using signature_type = void(error_qualified_type);

                if (not type_info<function_qualified_type>::template is_function<signature_type>())
                    return false;

                return true;
            }

            return false;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename error_type, typename function_type>
        constexpr auto on_error(function_type&& action) -> this_type
            requires(should_enable_on_error_function<this_type, function_type, error_type>())
        {
            _impl.template on_error<error_type>(forward<function_type>(action));
            return *this;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename final_qualified_type, typename function_qualified_type>
        static consteval auto should_enable_on_universal_error_function() -> bool
        {
            using this_type = type_info<final_qualified_type>::pure_type::value_type;

            if constexpr (type_info<this_type>::template is_derived_from<result_tag>())
            {
                if (not this_type::error_types_list::are_all(
                        [](auto info)
                        {
                            using error_qualified_type =
                                type_info<typename decltype(info)::value_type>::
                                    template unpure_like_type<final_qualified_type>::value_type;
                            using signature_type = void(error_qualified_type);

                            return type_info<function_qualified_type>::template is_function<
                                signature_type>;
                        }))
                    return false;

                return true;
            }

            return false;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename function_type>
        constexpr auto on_error(function_type&& action) -> this_type
            requires(should_enable_on_universal_error_function<this_type, function_type>())
        {
            _impl.on_error(forward<function_type>(action));
            return *this;
        }

    protected:
        impl_type _impl;
    };
}
