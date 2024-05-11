#pragma once
#include "atom/core/containers/dynamic_array.h"
#include "atom/core/core.h"

namespace atom
{
    template <typename allocator_t>
    class _buf_array_alloc_wrap: public allocator_t
    {};

    template <typename value_t, usize buf_size, typename allocator_t = default_mem_allocator>
    class buf_array: public dynamic_array<value_t, _buf_array_alloc_wrap<allocator_t>>
    {
        using base_t = dynamic_array<value_t, _buf_array_alloc_wrap<allocator_t>>;

    public:
        using base_t::base_t;
        using base_t::operator=;
    };
}
