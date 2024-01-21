export module atom.core:containers.buf_array;
import :containers.dynamic_array;
import :core;

namespace atom
{
    template <typename allocator_type>
    class _buf_array_alloc_wrap: public allocator_type
    {};

    export template <typename type, usize buf_size, typename allocator_type>
    class buf_array: public dynamic_array<type, _buf_array_alloc_wrap<allocator_type>>
    {
        using base_type = dynamic_array<type, _buf_array_alloc_wrap<allocator_type>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };
}
