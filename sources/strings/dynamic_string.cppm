export module atom.core:strings.dynamic_string;

import :containers;
import :strings.string_extensions;

namespace atom
{
    export template <typename allocator_type>
    class dynamic_string
        : public dynamic_array<char, allocator_type>
        , public string_extensions<dynamic_string<allocator_type>>
    {
        using base_type = dynamic_array<char, allocator_type>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };
}
