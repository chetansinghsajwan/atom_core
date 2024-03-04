#pragma once
#include "atom/core/string/string_functions.h"
#include "atom/core/containers/dynamic_array.h"

namespace atom
{
    template <typename allocator_type>
    class dynamic_string
        : public string_functions<dynamic_string<allocator_type>,
              dynamic_array<char, allocator_type>>
    {
        using base_type =
            string_functions<dynamic_string<allocator_type>, dynamic_array<char, allocator_type>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };
}
