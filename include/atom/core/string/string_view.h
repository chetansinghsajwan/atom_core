#pragma once
#include "atom/core/string/string_extensions.h"
#include "atom/core/containers/array_view.h"

namespace atom
{
    class string_view
        : public array_view<char>
        , public string_extensions<string_view>
    {
        using base_t = array_view<char>;

    public:
        using base_t::base_t;
        using base_t::operator=;
    };
}
