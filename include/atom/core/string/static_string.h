#pragma once
#include "atom/core/string/string_extensions.h"
// #include "atom/core/containers/static_array.h"

namespace atom
{
    template <usize size>
    class static_string
        : public static_array<char, size>
        , public string_extensions<static_string<size>>
    {
        using base_t = static_array<char, size>;

    public:
        using base_t::base_t;
        using base_t::operator=;
    };
}
