#pragma once
#include "atom/core/string/string_functions.h"
#include "atom/core/containers/array_view.h"

namespace atom
{
    class string_view: public string_functions<string_view, array_view<char>>
    {
        using base_type = string_functions<string_view, array_view<char>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };
}
