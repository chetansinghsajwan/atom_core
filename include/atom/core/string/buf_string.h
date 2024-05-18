#pragma once
// #include "atom/core/core.h"
#include "atom/core/string/string_extensions.h"
#include "atom/core/containers/buf_array.h"

namespace atom
{
    template <usize buf_size, typename allocator_t>
    class buf_string
        : public buf_array<char, buf_size, allocator_t>
        , public string_extensions<buf_string<buf_size, allocator_t>>
    {
        using base_t = buf_array<char, buf_size, allocator_t>;

    public:
        using base_t::base_t;
        using base_t::operator=;
    };
}
