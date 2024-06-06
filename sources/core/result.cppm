export module atom.core:core.result;

import :core.result_impl;
import :core.result_api;

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    export template <typename value_type, typename... error_types>
    class result: public result_api<result_impl<value_type, error_types...>>
    {
        using this_type = result;
        using base_type = result_api<result_impl<value_type, error_types...>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };
}
