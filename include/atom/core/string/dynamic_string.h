#pragma once
#include "atom/core/string/string_functions.h"
#include "atom/core/containers/dynamic_array.h"

namespace atom
{
    template <typename allocator_t>
    class dynamic_string
        : public string_functions<dynamic_string<allocator_t>,
              dynamic_array<char, allocator_t>>
    {
        using base_t =
            string_functions<dynamic_string<allocator_t>, dynamic_array<char, allocator_t>>;

    public:
        using base_t::base_t;
        using base_t::operator=;
    };
}
