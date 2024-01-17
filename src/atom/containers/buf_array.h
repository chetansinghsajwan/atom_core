#pragma once
#include "dynamic_array.h"

namespace atom
{
    template <typename allocator_type>
    class _buf_array_alloc_wrap: public allocator_type
    {};

    template <typename type, usize buf_size, typename allocator_type>
    class buf_array: public dynamic_array<type, _buf_array_alloc_wrap<allocator_type>>
    {
        using base_type = dynamic_array<type, _buf_array_alloc_wrap<allocator_type>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };
}
