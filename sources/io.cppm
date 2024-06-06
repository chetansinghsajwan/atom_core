export module atom.core:io;

import std;
import :core;
import :filesystem;
import :strings;

export namespace atom::io
{
    filesystem::file stdin{ ::stdin, filesystem::file::open_flags::read };
    filesystem::file stdout{ ::stdout, filesystem::file::open_flags::write };
    filesystem::file stderr{ ::stderr, filesystem::file::open_flags::write };

    template <typename... arg_types>
    inline auto print(format_string<arg_types...> fmt, arg_types&&... args)
    {
        stdout.write_fmt(fmt, forward<arg_types>(args)...);
    }

    template <typename... arg_types>
    inline auto println(format_string<arg_types...> fmt, arg_types&&... args)
    {
        stdout.write_line_fmt(fmt, forward<arg_types>(args)...);
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
