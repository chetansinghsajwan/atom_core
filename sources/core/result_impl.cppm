export module atom.core:core.result_impl;

import :types;
import :contracts;
import :core.core;
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
            typeutils::conditional_type<typeinfo<value_type>::template is_same_as<_result_void>, void,
                value_type>;
        using error_types_list = typelist<error_types...>;
        using first_error_type = typename error_types_list::front_type;

        static_assert(error_types_list::count != 0);

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
        constexpr auto emplace_value(this this_type& self, args_type&&... args) -> void
        {
            self._variant.template emplace_value_by_index<value_index>(forward<args_type>(args)...);
        }

        constexpr auto set_value(this this_type& self, const value_type& value) -> void
            requires(not typeinfo<value_type>::is_void)
        {
            self._variant.template emplace_value_by_index<value_index>(value);
        }

        constexpr auto set_value(this this_type& self, value_type&& value) -> void
            requires(not typeinfo<value_type>::is_void)
        {
            self._variant.template emplace_value_by_index<value_index>(move(value));
        }

        constexpr auto set_value_void(this this_type& self) -> void
            requires(not typeinfo<value_type>::is_void)
        {
            self._variant.template emplace_value_by_index<value_index>();
        }

        constexpr auto get_value(this const this_type& self) -> const value_type&
        {
            return self._variant.template get_at<value_index>();
        }

        constexpr auto get_value(this this_type& self) -> value_type&
        {
            return self._variant.template get_at<value_index>();
        }

        constexpr auto get_value_checked() -> value_type&
        {
            return _variant.template get_at_checked<value_index>();
        }

        constexpr auto get_value_checked() const -> const value_type&
        {
            return _variant.template get_at_checked<value_index>();
        }

        constexpr auto is_value(this const this_type& self) -> bool
        {
            return self._variant.template is<value_type>();
        }

        consteval auto get_error_count() -> usize
        {
            return _variant_type::get_type_count() - 1;
        }

        template <typename error_type>
        static consteval auto has_error() -> bool
        {
            if (typeinfo<error_type>::template is_same_as<value_type>)
                return false;

            return _variant_type::template has<error_type>();
        }

        template <typename error_type, typename... args_type>
        constexpr auto emplace_error(this this_type& self, args_type&&... args) -> void
        {
            self._variant.template emplace<error_type>(forward<args_type>(args)...);
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
        constexpr auto get_error(this const this_type& self) -> const error_type&
        {
            return self._variant.template get<error_type>();
        }

        template <typename error_type>
        constexpr auto get_error(this this_type& self) -> error_type&
        {
            return self._variant.template as<error_type>();
        }

        constexpr auto get_first_error(this const this_type& self) -> const first_error_type&
        {
            return self._variant.template get<first_error_type>();
        }

        constexpr auto get_first_error(this this_type& self) -> first_error_type&
        {
            return self._variant.template get_mut<first_error_type>();
        }

        constexpr auto is_error(this const this_type& self) -> bool
        {
            return not self._variant.template is<value_type>();
        }

        template <typename error_type>
        constexpr auto is_error(this const this_type& self) -> bool
        {
            return self._variant.template is<error_type>();
        }

        constexpr auto panic_on_error(this const this_type& self) -> void
        {
            if (self.is_error())
                contract_panic();
        }

        constexpr auto on_value(this auto& self, auto&& action) -> void
        {
            if (self.is_value())
            {
                // action(self.get_value());
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

        constexpr auto to_option(this auto&& self) -> option<value_type>
        {
            if (self.is_error())
                return nullopt();

            return option(self.get_value());
        }

        template <typename error_type>
        constexpr auto to_option_error(this auto&& self) -> option<error_type>
        {
            if (self.template is_error<error_type>())
                return nullopt();

            return option(self.template get_error<error_type>());
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
