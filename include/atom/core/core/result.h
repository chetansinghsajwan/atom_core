#pragma once
#include "atom/core/core/result_api.h"
#include "atom/core/core/result_impl.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename value_t, typename... error_ts>
    class result: public result_api<result_impl<value_t, error_ts...>>
    {
        using this_t = result;
        using base_t = result_api<result_impl<value_t, error_ts...>>;

    public:
        using base_t::base_t;
        using base_t::operator=;
    };
}
