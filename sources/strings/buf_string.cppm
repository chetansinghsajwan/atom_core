export module atom.core:strings.buf_string;

import :core;
import :containers;
import :strings.string_extensions;

namespace atom
{
    export template <usize buf_size, typename allocator_type>
    class buf_string
        : public buf_array<char, buf_size, allocator_type>
        , public string_extensions<buf_string<buf_size, allocator_type>>
    {
        using base_type = buf_array<char, buf_size, allocator_type>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };
}
