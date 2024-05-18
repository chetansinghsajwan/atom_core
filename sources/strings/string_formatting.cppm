export module atom.core:strings.string_formatting;

import std;
import fmt;
import :core;
import :types;
import :strings.string_formatter;
import :strings.string_format_context;
import :strings.string_formatter_provider;
import :strings.string_format_error;
import :strings.format_string;
import :strings._format_arg_wrapper;

namespace atom
{
    constexpr auto _fmt_error_to_string_format_error(const fmt::format_error& err)
        -> string_format_error
    {
        return string_format_error("conversion from fmt::format_error to atom::string_format_error "
                                   "is not implemented yet.");
    }

    template <typename... arg_ts>
    constexpr auto _convert_format_string_atom_to_fmt(format_string<arg_ts...> fmt)
    {
        return fmt::runtime(fmt.str.to_std());
    }

    template <typename output_t>
    constexpr auto _wrap_output_t_atom_to_fmt(output_t* out)
    {
        class _wrapper
        {
        public:
            using valueue_t = char;
            using difference_t = std::ptrdiff_t;
            using pointer = char*;
            using reference = char&;
            using iterator_category = std::output_iterator_tag;

        public:
            constexpr auto operator++() -> _wrapper&
            {
                return *this;
            }

            constexpr auto operator++(int) -> _wrapper&
            {
                return *this;
            }

            constexpr auto operator*() -> _wrapper&
            {
                return *this;
            }

            constexpr auto operator=(char ch) -> _wrapper&
            {
                *out += char(ch);
                return *this;
            }

        public:
            typeinfo<output_t>::pure_t::value_t* out;
        };

        return _wrapper(out);
    }

    template <typename output_t, typename... arg_ts>
    constexpr auto _format_to(output_t&& out, format_string<arg_ts...> fmt, arg_ts&&... args)
    {
        try
        {
            fmt::format_to(_wrap_output_t_atom_to_fmt(&out),
                _convert_format_string_atom_to_fmt<arg_ts...>(fmt),
                _format_arg_wrapper(atom::forward<arg_ts>(args))...);
        }
        catch (const fmt::format_error& err)
        {
            throw _fmt_error_to_string_format_error(err);
        }
    }
}
