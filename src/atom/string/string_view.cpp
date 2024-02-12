export module atom.core:string_view;
import :string_functions;
import :containers.array_view;

namespace atom
{
    export class string_view: public string_functions<array_view>
    {
        using base_type = string_functions<array_view>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };
}
