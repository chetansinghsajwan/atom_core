#pragma once
#include "atom/core/string/string_extensions.h"
#include "atom/core/containers/array_slice.h"

namespace atom
{
    class string_slice
        : public array_slice<char>
        , public string_extensions<string_slice>
    {
        using base_t = array_slice<char>;

    public:
        using base_t::base_t;
        using base_t::operator=;
    };
}
