#pragma once
#include "atom/core/core.h"
#include "atom/core/string/string_functions.h"
#include "atom/core/containers/buf_array.h"

namespace atom
{
    template <usize buf_size, typename allocator_t>
    class buf_string
        : public string_functions<buf_string<buf_size, allocator_t>,
              buf_array<char, buf_size, allocator_t>>
    {
        using base_t = string_functions<buf_string<buf_size, allocator_t>,
            buf_array<char, buf_size, allocator_t>>;

    public:
        using base_t::base_t;
        using base_t::operator=;
    };
}
