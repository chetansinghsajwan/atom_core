#pragma once
#include "atom/core/string/string_functions.h"
#include "atom/core/containers/array_view.h"

namespace atom
{
    class string_view: public string_functions<string_view, array_view<char>>
    {
        using base_t = string_functions<string_view, array_view<char>>;

    public:
        using base_t::base_t;
        using base_t::operator=;
    };
}
