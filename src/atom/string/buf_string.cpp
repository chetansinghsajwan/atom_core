export module atom.core:buf_string;
import :string_functions;
import :containers.buf_array;
import :core;

namespace atom
{
    export template <usize buf_size, typename allocator_type>
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
