#pragma once
#include "variant.h"

namespace atom
{
    template <typename type, typename... terrs>
    class result: public variant<type, terrs...>
    {
        using base_type = variant<type, terrs...>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

    class _result_void
    {};

    template <typename... terrs>
    class result<void, terrs...>: public variant<_result_void, terrs...>
    {
        using base_type = variant<_result_void, terrs...>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };

    template <typename... terrs>
    constexpr auto success() -> result<void, terrs...>
    {
        return { _result_void{} };
    }
}
