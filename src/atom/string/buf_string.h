#pragma once
#include "_string_impl.h"
#include "atom/containers/buf_array.h"

namespace atom
{
    template <usize buf_size, typename allocator_type>
    class buf_string_container_provider
    {
    public:
        template <typename value_type>
        using type = buf_array<value_type, buf_size, allocator_type>;
    };

    template <usize buf_size, typename allocator_type>
    using buf_string =
        _string_impl<buf_string_container_provider<buf_size, allocator_type>::template type>;
}
