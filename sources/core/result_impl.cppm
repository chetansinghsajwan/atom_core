export module atom.core:core.result_impl;

import :types;
import :core.core;
import :core.nums;
import :core.option;
import :core.variant_impl;

namespace atom
{
    class _result_void
    {};

    template <typename in_value_type, typename... error_types>
    class result_impl
    {
        template <typename that_value_type, typename... that_error_types>
        friend class result_impl;

    private:
        using this_type = result_impl;
        using impl_type = variant_impl<in_value_type, error_types...>;

    public:
        using value_type = in_value_type;
        using out_value_type =
            type_utils::conditional_type<type_info<value_type>::template is_same_as<_result_void>(),
                void, value_type>;
        using error_types_list = type_list<error_types...>;

    public:
        class that_tag
        {};

        template <typename value_type>
        class emplace_tag
        {};

    public:
        constexpr result_impl() = delete;

        constexpr result_impl(const this_type& that) = default;

        constexpr result_impl(that_tag, const this_type& that)
            : _impl{ typename impl_type::that_tag{}, that._impl }
        {}

        template <typename that_type>
        constexpr result_impl(that_tag, const that_type& that)
            : _impl{ typename impl_type::that_tag{}, that._impl }
        {}

        constexpr result_impl(this_type&& that) = default;

        constexpr result_impl(that_tag, this_type&& that)
            : _impl{ typename impl_type::that_tag{}, move(that._impl) }
        {}

        template <typename that_type>
        constexpr result_impl(that_tag, that_type&& that)
            : _impl{ typename impl_type::that_tag{}, move(that._impl) }
        {}

        constexpr result_impl(emplace_tag<void>)
            : _impl{ typename impl_type::template emplace_tag<void>{} }
        {}

        constexpr result_impl(emplace_tag<value_type>, const value_type& value)
            : _impl{ typename impl_type::template emplace_tag<value_type>{}, value }
        {}

        constexpr result_impl(emplace_tag<value_type>, value_type&& value)
            : _impl{ typename impl_type::template emplace_tag<value_type>{}, move(value) }
        {}

        template <typename error_type>
        constexpr result_impl(emplace_tag<error_type>, const error_type& error)
            : _impl{ typename impl_type::template emplace_tag<error_type>{}, error }
        {}

        template <typename error_type>
        constexpr result_impl(emplace_tag<error_type>, error_type&& error)
            : _impl{ typename impl_type::template emplace_tag<error_type>{}, move(error) }
        {}

        constexpr ~result_impl() = default;

    public:
        template <typename... arg_types>
        constexpr auto emplace_value(arg_types&&... args) -> void
        {
            _impl.template emplace_value<value_type>(forward<arg_types>(args)...);
        }

        constexpr auto set_value(const value_type& value) -> void
            requires(not type_info<value_type>::is_void())
        {
            _impl.template emplace_value<value_type>(value);
        }

        constexpr auto set_value(value_type&& value) -> void
            requires(not type_info<value_type>::is_void())
        {
            _impl.template emplace_value<value_type>(move(value));
        }

        constexpr auto set_value_void() -> void
            requires(not type_info<value_type>::is_void())
        {
            _impl.template emplace_value<value_type>();
        }

        constexpr auto get_value() -> value_type&
        {
            return _impl.template get_value<value_type>();
        }

        constexpr auto get_value() const -> const value_type&
        {
            return _impl.template get_value<value_type>();
        }

        constexpr auto is_value() const -> bool
        {
            return _impl.template is_type<value_type>();
        }

        template <typename error_type, typename... arg_types>
        constexpr auto emplace_error(arg_types&&... args) -> void
        {
            _impl.template emplace_value<error_type>(forward<arg_types>(args)...);
        }

        template <typename error_type>
        constexpr auto set_error(const error_type& error) -> void
        {
            _impl.template emplace_value<error_type>(error);
        }

        template <typename error_type>
        constexpr auto set_error(error_type&& error) -> void
        {
            _impl.template emplace_value<error_type>(move(error));
        }

        template <typename error_type>
        constexpr auto get_error() -> error_type&
        {
            return _impl.template get_value<error_type>();
        }

        template <typename error_type>
        constexpr auto get_error() const -> const error_type&
        {
            return _impl.template get_value<error_type>();
        }

        constexpr auto is_error() const -> bool
        {
            return not _impl.template is_type<value_type>();
        }

        template <typename error_type>
        constexpr auto is_error() const -> bool
        {
            return _impl.template is_type<error_type>();
        }

        template <typename... other_error_types>
        constexpr auto is_error_any() const -> bool
        {
            return _impl.template is_any_type<other_error_types...>();
        }

        constexpr auto to_option() -> option<value_type>
        {
            if (is_error())
                return nullopt();

            return option{ create_by_emplace<value_type>, get_value() };
        }

        template <typename error_type>
        constexpr auto to_option_error() -> option<error_type>
        {
            if (is_error<error_type>())
                return nullopt();

            return option{ create_by_emplace<error_type>, get_error<error_type>() };
        }

        template <typename that_type>
        constexpr auto is_eq(const that_type& that) const -> bool
        {
            return _impl.is_eq(that._impl);
        }

    private:
        impl_type _impl;
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
