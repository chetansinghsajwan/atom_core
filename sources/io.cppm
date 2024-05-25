export module atom.core:io;

import std;
import :filesystem;
import :strings;

namespace atom::io
{
    export filesystem::file stdin{ ::stdin, filesystem::file::open_flags::read };
    export filesystem::file stdout{ ::stdout, filesystem::file::open_flags::write };
    export filesystem::file stderr{ ::stderr, filesystem::file::open_flags::write };

    template <typename... arg_types>
    inline auto print(format_string<arg_types...> fmt, arg_types&&... args)
    {
        stdout.write_str(fmt, forward<arg_types>(args)...);
    }

    template <typename... arg_types>
    inline auto println(format_string<arg_types...> fmt, arg_types&&... args)
    {
        stdout.write_line(fmt, forward<arg_types>(args)...);
    }

    template <typename value_type>
    auto print(const value_type& value)
        requires(string_formatter_provider<value_type>::has())
    {
        stdout.write_str("{}", value);
    }

    template <typename value_type>
    auto println(const value_type& value)
        requires(string_formatter_provider<value_type>::has())
    {
        stdout.write_line("{}", value);
    }
}
