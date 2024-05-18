export module atom.core:strings.string_view;

import :containers;
import :strings.string_extensions;

namespace atom
{
    export class string_view
        : public array_view<char>
        , public string_extensions<string_view>
    {
        using base_t = array_view<char>;

    public:
        using base_t::base_t;
        using base_t::operator=;
    };
}
