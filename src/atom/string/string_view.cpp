export module atom.core:string_view;
import :string_functions;
import :containers.array_view;

namespace atom
{
    export class string_view: public string_functions<string_view, array_view<char>>
    {
        using base_type = string_functions<string_view, array_view<char>>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };
}
