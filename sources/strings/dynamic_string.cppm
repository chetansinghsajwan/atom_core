export module atom.core:strings.dynamic_string;

import :containers;
import :strings.string_extensions;

namespace atom
{
    export template <typename allocator_t>
    class dynamic_string
        : public dynamic_array<char, allocator_t>
        , public string_extensions<dynamic_string<allocator_t>>
    {
        using base_t = dynamic_array<char, allocator_t>;

    public:
        using base_t::base_t;
        using base_t::operator=;
    };
}
