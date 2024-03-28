#pragma once
#include "atom/core/core/variant.h"

namespace atom
{
    template <typename value_t, typename... error_ts>
    class result: public variant<value_t, error_ts...>
    {
        using this_t = result;
        using base_t = variant<value_t, error_ts...>;

    public:
        using base_t::base_t;
        using base_t::operator=;

    public:
        constexpr auto get_value(this const this_t& self) -> const value_t&
        {
            return self.base_t::template get_at<0>();
        }

        constexpr auto get_value(this this_t& self) -> value_t&
        {
            return self.base_t::template get_mut_at<0>();
        }

        constexpr auto get_mut_value(this this_t& self) -> value_t&
        {
            return self.base_t::template get_mut_at<0>();
        }

        constexpr auto is_value(this const this_t& self) -> bool
        {
            return self.base_t::template is<0>();
        }

        constexpr auto is_error(this const this_t& self) -> bool
        {
            return not self.is_value();
        }

        constexpr auto panic_on_error(this const this_t& self) -> void
        {
            if (self.is_error())
            {
                ATOM_PANIC();
            }
        }
    };

    class _result_void
    {};

    template <typename... error_ts>
    class result<void, error_ts...>: public variant<_result_void, error_ts...>
    {
        using this_t = result;
        using base_t = variant<_result_void, error_ts...>;

    public:
        using base_t::base_t;
        using base_t::operator=;

    public:
        constexpr auto is_value(this const this_t& self) -> bool
        {
            return self.base_t::template is<0>();
        }

        constexpr auto is_error(this const this_t& self) -> bool
        {
            return not self.is_value();
        }

        constexpr auto panic_on_error(this const this_t& self) -> void
        {
            if (self.is_error())
            {
                ATOM_PANIC();
            }
        }
    };

    template <typename... error_ts>
    constexpr auto success() -> result<void, error_ts...>
    {
        using value_t = result<void, error_ts...>;
        return value_t(_result_void());
    }
}
