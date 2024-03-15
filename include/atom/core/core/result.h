#pragma once
#include "atom/core/core/variant.h"

namespace atom
{
    template <typename result_t, typename... error_ts>
    class result: public variant<result_t, error_ts...>
    {
        using base_t = variant<result_t, error_ts...>;

    public:
        using base_t::base_t;
        using base_t::operator=;
    };

    class _result_void
    {};

    template <typename... error_ts>
    class result<void, error_ts...>: public variant<_result_void, error_ts...>
    {
        using base_t = variant<_result_void, error_ts...>;

    public:
        using base_t::base_t;
        using base_t::operator=;
    };

    template <typename... error_ts>
    constexpr auto success() -> result<void, error_ts...>
    {
        using result_t = result<void, error_ts...>;
        return result_t(_result_void());
    }
}
