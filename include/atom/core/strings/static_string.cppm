export module atom.core:strings.static_string;

import :containers;
import :strings.string_extensions;

namespace atom
{
    export template <usize size>
    class static_string
        : public static_array<char, size>
        , public string_extensions<static_string<size>>
    {
        using base_t = static_array<char, size>;

    public:
        using base_t::base_t;
        using base_t::operator=;
    };
}
