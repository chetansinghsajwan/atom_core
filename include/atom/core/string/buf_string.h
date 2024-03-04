#pragma once
#include "atom/core/core.h"
#include "atom/core/string/string_functions.h"
#include "atom/core/containers/buf_array.h"

namespace atom
{
    template <usize buf_size, typename allocator_type>
    class buf_string
        : public string_functions<buf_string<buf_size, allocator_type>,
              buf_array<char, buf_size, allocator_type>>
    {
        using base_type = string_functions<buf_string<buf_size, allocator_type>,
            buf_array<char, buf_size, allocator_type>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };
}
