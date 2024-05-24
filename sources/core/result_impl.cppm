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

    template <typename in_value_t, typename... error_ts>
    class result_impl
    {
        using this_t = result_impl;

        template <typename that_value_t, typename... that_error_ts>
        friend class result_impl;

    protected:
        using _variant_t = variant<in_value_t, error_ts...>;

    public:
        using value_t = in_value_t;
        using out_value_t =
            typeutils::conditional_t<typeinfo<value_t>::template is_same_as<_result_void>, void,
                value_t>;
        using error_types_list = typelist<error_ts...>;
        using first_error_t = typename error_types_list::front_t;

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

        constexpr result_impl(const this_t& that) = default;

        constexpr result_impl(copy_tag, const this_t& that)
            : _variant(that._variant)
        {}

        template <typename that_t>
        constexpr result_impl(copy_tag, const that_t& that)
            : _variant(that._variant)
        {}

        constexpr result_impl(this_t&& that) = default;

        constexpr result_impl(move_tag, this_t&& that)
            : _variant(move(that))
        {}

        template <typename that_t>
        constexpr result_impl(move_tag, that_t&& that)
            : _variant(move(that))
        {}

        constexpr result_impl(value_tag, const value_t& value)
            : _variant(value)
        {}

        constexpr result_impl(value_tag, value_t&& value)
            : _variant(move(value))
        {}

        template <typename error_t>
        constexpr result_impl(error_tag, const error_t& error)
            : _variant(error)
        {}

        template <typename error_t>
        constexpr result_impl(error_tag, error_t&& error)
            : _variant(move(error))
        {}

        constexpr ~result_impl() = default;

    public:
        template <typename... args_t>
        constexpr auto emplace_value(this this_t& self, args_t&&... args) -> void
        {
            self._variant.template emplace_value_by_index<value_index>(forward<args_t>(args)...);
        }

        constexpr auto set_value(this this_t& self, const value_t& value) -> void
            requires(not typeinfo<value_t>::is_void)
        {
            self._variant.template emplace_value_by_index<value_index>(value);
        }

        constexpr auto set_value(this this_t& self, value_t&& value) -> void
            requires(not typeinfo<value_t>::is_void)
        {
            self._variant.template emplace_value_by_index<value_index>(move(value));
        }

        constexpr auto set_value_void(this this_t& self) -> void
            requires(not typeinfo<value_t>::is_void)
        {
            self._variant.template emplace_value_by_index<value_index>();
        }

        constexpr auto get_value(this const this_t& self) -> const value_t&
            requires(not typeinfo<value_t>::is_void)
        {
            return self._variant.template get_at<value_index>();
        }

        constexpr auto get_value(this this_t& self) -> value_t&
            requires(not typeinfo<value_t>::is_void)
        {
            return self._variant.template get_at<value_index>();
        }

        constexpr auto is_value(this const this_t& self) -> bool
        {
            return self._variant.template is<value_t>();
        }

        consteval auto get_error_count() -> usize
        {
            return _variant_t::get_type_count() - 1;
        }

        template <typename error_t>
        static consteval auto has_error() -> bool
        {
            if (typeinfo<error_t>::template is_same_as<value_t>)
                return false;

            return _variant_t::template has<error_t>();
        }

        template <typename error_t, typename... args_t>
        constexpr auto emplace_error(this this_t& self, args_t&&... args) -> void
        {
            self._variant.template emplace<error_t>(forward<args_t>(args)...);
        }

        template <typename error_t>
        constexpr auto set_error(this this_t& self, const error_t& error) -> void
        {
            self._variant.template emplace<error_t>(error);
        }

        template <typename error_t>
        constexpr auto set_error(this this_t& self, error_t&& error) -> void
        {
            self._variant.template emplace<error_t>(move(error));
        }

        template <typename error_t>
        constexpr auto get_error(this const this_t& self) -> const error_t&
        {
            return self._variant.template get<error_t>();
        }

        template <typename error_t>
        constexpr auto get_error(this this_t& self) -> error_t&
        {
            return self._variant.template get_mut<error_t>();
        }

        constexpr auto get_first_error(this const this_t& self) -> const first_error_t&
        {
            return self._variant.template get<first_error_t>();
        }

        constexpr auto get_first_error(this this_t& self) -> first_error_t&
        {
            return self._variant.template get_mut<first_error_t>();
        }

        constexpr auto is_error(this const this_t& self) -> bool
        {
            return not self._variant.template is<value_t>();
        }

        template <typename error_t>
        constexpr auto is_error(this const this_t& self) -> bool
        {
            return self._variant.template is<error_t>();
        }

        constexpr auto panic_on_error(this const this_t& self) -> void
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

        template <typename function_t>
        constexpr auto on_error(function_t&& action) -> void
        {
            if (is_error())
            {
                // action(get_error());
            }
        }

        template <typename error_t, typename function_t>
        constexpr auto on_error(function_t&& action) -> void
        {
            if (is_error())
            {
                action(get_error<error_t>());
            }
        }

        constexpr auto to_option(this auto&& self) -> option<value_t>
        {
            if (self.is_error())
                return nullopt();

            return option(self.get_value());
        }

        template <typename error_t>
        constexpr auto to_option_error(this auto&& self) -> option<error_t>
        {
            if (self.template is_error<error_t>())
                return nullopt();

            return option(self.template get_error<error_t>());
        }

    protected:
        _variant_t _variant;
    };

    template <typename... error_ts>
    class result_impl<void, error_ts...>: public result_impl<_result_void, error_ts...>
    {
        using base_t = result_impl<_result_void, error_ts...>;

    public:
        using base_t::base_t;
        using base_t::operator=;
    };
}
