export module atom.core:string_view;
import :_string_impl;
import :containers.array_view;

namespace atom
{
    export class string_view: public _string_impl<array_view>
    {
        using base_type = _string_impl<array_view>;

    public:
        using base_type::base_type;
        using base_type::operator=;
    };
}
