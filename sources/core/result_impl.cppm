export module atom.core:core.result_impl;

import :types;
import :contracts;
import :core.core;
import :core.nums;
import :core.option;
import :core.variant;

namespace atom
{
    class _result_void
    {};

    template <typename in_value_type, typename... error_types>
    class result_impl
    {
        using this_type = result_impl;

        template <typename that_value_type, typename... that_error_types>
        friend class result_impl;

    protected:
        using _variant_type = variant<in_value_type, error_types...>;

    public:
        using value_type = in_value_type;
        using out_value_type =
            type_utils::conditional_type<type_info<value_type>::template is_same_as<_result_void>(), void,
                value_type>;
        using error_type_list = type_list<error_types...>;

        static_assert(not error_type_list::is_empty());

        class copy_tag
        {};

        class move_tag
        {};

        class value_tag
        {};

        class error_tag
        {};

        static constexpr usize value_index = 0;

    public:
        constexpr result_impl() = delete;

        constexpr result_impl(const this_type& that) = default;

        constexpr result_impl(copy_tag, const this_type& that)
            : _variant(that._variant)
        {}

        template <typename that_type>
        constexpr result_impl(copy_tag, const that_type& that)
            : _variant(that._variant)
        {}

        constexpr result_impl(this_type&& that) = default;

        constexpr result_impl(move_tag, this_type&& that)
            : _variant(move(that._variant))
        {}

        template <typename that_type>
        constexpr result_impl(move_tag, that_type&& that)
            : _variant(move(that._variant))
        {}

        constexpr result_impl(value_tag, const value_type& value)
            : _variant(value)
        {}

        constexpr result_impl(value_tag, value_type&& value)
            : _variant(move(value))
        {}

        template <typename error_type>
        constexpr result_impl(error_tag, const error_type& error)
            : _variant(error)
        {}

        template <typename error_type>
        constexpr result_impl(error_tag, error_type&& error)
            : _variant(move(error))
        {}

        constexpr ~result_impl() = default;

    public:
        template <typename... args_type>
        constexpr auto emplace_value(args_type&&... args) -> void
        {
            _variant.template emplace_value_by_index<value_index>(forward<args_type>(args)...);
        }

        constexpr auto set_value(const value_type& value) -> void
            requires(not type_info<value_type>::is_void)
        {
            _variant.template emplace_value_by_index<value_index>(value);
        }

        constexpr auto set_value(value_type&& value) -> void
            requires(not type_info<value_type>::is_void)
        {
            _variant.template emplace_value_by_index<value_index>(move(value));
        }

        constexpr auto set_value_void() -> void
            requires(not type_info<value_type>::is_void)
        {
            _variant.template emplace_value_by_index<value_index>();
        }

        constexpr auto get_value() const -> const value_type&
        {
            return _variant.template get_at<value_index>();
        }

        constexpr auto get_value() -> value_type&
        {
            return _variant.template get_at<value_index>();
        }

        constexpr auto get_value_checked() -> value_type&
        {
            return _variant.template get_at_checked<value_index>();
        }

        constexpr auto get_value_checked() const -> const value_type&
        {
            return _variant.template get_at_checked<value_index>();
        }

        constexpr auto is_value() const -> bool
        {
            return _variant.template is<value_type>();
        }

        consteval auto get_error_count() -> usize
        {
            return _variant_type::get_type_count() - 1;
        }

        template <typename error_type>
        static consteval auto has_error() -> bool
        {
            if (type_info<error_type>::template is_same_as<value_type>())
                return false;

            return _variant_type::template has<error_type>();
        }

        template <typename error_type, typename... args_type>
        constexpr auto emplace_error(args_type&&... args) -> void
        {
            _variant.template emplace<error_type>(forward<args_type>(args)...);
        }

        template <typename error_type>
        constexpr auto set_error(const error_type& error) -> void
        {
            _variant.template emplace<error_type>(error);
        }

        template <typename error_type>
        constexpr auto set_error(error_type&& error) -> void
        {
            _variant.template emplace<error_type>(move(error));
        }

        template <typename error_type>
        constexpr auto get_error() const -> const error_type&
        {
            return _variant.template get<error_type>();
        }

        template <typename error_type>
        constexpr auto get_error() -> error_type&
        {
            return _variant.template as<error_type>();
        }

        constexpr auto is_error() const -> bool
        {
            return not _variant.template is<value_type>();
        }

        template <typename error_type>
        constexpr auto is_error() const -> bool
        {
            return _variant.template is<error_type>();
        }

        template <typename... other_error_types>
        constexpr auto is_error_any() const -> bool
        {
            return _variant.template is_any<other_error_types...>();
        }

        constexpr auto panic_on_error() const -> void
        {
            if (is_error())
                contract_panic();
        }

        constexpr auto on_value(auto&& action) -> void
        {
            if (is_value())
            {
                // action(get_value());
            }
        }

        template <typename function_type>
        constexpr auto on_error(function_type&& action) -> void
        {
            if (is_error())
            {
                // action(get_error());
            }
        }

        template <typename error_type, typename function_type>
        constexpr auto on_error(function_type&& action) -> void
        {
            if (is_error())
            {
                action(get_error<error_type>());
            }
        }

        constexpr auto to_option() -> option<value_type>
        {
            if (is_error())
                return nullopt();

            return option(get_value());
        }

        template <typename error_type>
        constexpr auto to_option_error() -> option<error_type>
        {
            if (is_error<error_type>())
                return nullopt();

            return option(get_error<error_type>());
        }

    protected:
        _variant_type _variant;
    };

    template <typename... error_types>
    class result_impl<void, error_types...>: public result_impl<_result_void, error_types...>
    {
        using base_type = result_impl<_result_void, error_types...>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };
}
