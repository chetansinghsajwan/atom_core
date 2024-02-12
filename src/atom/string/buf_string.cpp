export module atom.core:buf_string;
import :string_functions;
import :containers.buf_array;
import :core;

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
        string_functions<buf_string_container_provider<buf_size, allocator_type>::template type>;
}
