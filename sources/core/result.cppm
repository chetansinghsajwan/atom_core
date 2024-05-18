export module atom.core:core.result;

import :core.result_impl;
import :core.result_api;

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    export template <typename value_t, typename... error_ts>
    class result: public result_api<result_impl<value_t, error_ts...>>
    {
        using this_t = result;
        using base_t = result_api<result_impl<value_t, error_ts...>>;

    public:
        using base_t::base_t;
        using base_t::operator=;
    };
}
