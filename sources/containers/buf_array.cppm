export module atom.core:containers.buf_array;

import :core;
import :ranges;
import :default_mem_allocator;
import :containers.dynamic_array;

namespace atom
{
    template <typename allocator_type>
    class _buf_array_alloc_wrap: public allocator_type
    {};

    export template <typename value_type, usize buf_size,
        typename allocator_type = default_mem_allocator>
    class buf_array: public dynamic_array<value_type, _buf_array_alloc_wrap<allocator_type>>
    {
        using base_type = dynamic_array<value_type, _buf_array_alloc_wrap<allocator_type>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };
}
