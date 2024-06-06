export module atom.core:strings.string_view;

import :containers;
import :strings.string_extensions;

namespace atom
{
    export class string_view
        : public array_view<char>
        , public string_extensions<string_view>
    {
        using base_type = array_view<char>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };
}
