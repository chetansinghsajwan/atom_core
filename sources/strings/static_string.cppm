export module atom_core:strings.static_string;

import :core;
import :containers;
import :strings.string_tag;

namespace atom
{
    export template <usize size>
    class static_string
        : public static_array<char, size>
        , public string_tag
    {
        using base_type = static_array<char, size>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };
}
