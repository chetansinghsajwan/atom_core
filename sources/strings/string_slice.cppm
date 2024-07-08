export module atom_core:strings.string_slice;

import :containers;
import :strings.string_tag;

namespace atom
{
    export class string_slice
        : public array_slice<char>
        , public string_tag
    {
        using base_type = array_slice<char>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };
}
