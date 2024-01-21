export module atom.core:string_view;
import :_string_impl;
import :containers.array_view;

namespace atom
{
    export using string_view = _string_impl<array_view>;
}
