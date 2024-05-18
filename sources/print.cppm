export module atom.core:print;

import fmt;
import :core;
import :strings;

export namespace atom
{
    template <typename... arg_types>
    inline auto print(format_string<arg_types...> fmt, arg_types&&... args)
    {
        fmt::print(_convert_format_string_atom_to_fmt(fmt), forward<arg_types>(args)...);
    }

    template <typename... arg_types>
    inline auto println(format_string<arg_types...> fmt, arg_types&&... args)
    {
        fmt::print(_convert_format_string_atom_to_fmt(fmt), forward<arg_types>(args)...);
    }
}
