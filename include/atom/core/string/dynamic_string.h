#pragma once
#include "atom/core/string/string_extensions.h"
// #include "atom/core/containers/dynamic_array.h"

namespace atom
{
    template <typename allocator_t>
    class dynamic_string
        : public dynamic_array<char, allocator_t>
        , public string_extensions<dynamic_string<allocator_t>>
    {
        using base_t = dynamic_array<char, allocator_t>;

    public:
        using base_t::base_t;
        using base_t::operator=;
    };
}
