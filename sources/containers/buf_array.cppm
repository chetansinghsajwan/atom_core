export module atom.core:containers.buf_array;

import :core;
import :ranges;
import :default_mem_allocator;
import :containers.dynamic_array;

namespace atom
{
    template <typename allocator_t>
    class _buf_array_alloc_wrap: public allocator_t
    {};

    export template <typename value_t, usize buf_size, typename allocator_t = default_mem_allocator>
    class buf_array: public dynamic_array<value_t, _buf_array_alloc_wrap<allocator_t>>
    {
        using base_t = dynamic_array<value_t, _buf_array_alloc_wrap<allocator_t>>;

    public:
        using base_t::base_t;
        using base_t::operator=;
    };
}
