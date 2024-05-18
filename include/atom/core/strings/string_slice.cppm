export module atom.core:strings.string_slice;

import :containers;
import :strings.string_extensions;

namespace atom
{
    export class string_slice
        : public array_slice<char>
        , public string_extensions<string_slice>
    {
        using base_t = array_slice<char>;

    public:
        using base_t::base_t;
        using base_t::operator=;
    };
}
