#pragma once
#include "atom/core/string/_format_arg_wrapper.h"
#include "atom/core/string/format_string.h"
#include "atom/core/string/string_format_error.h"
#include "atom/core/string/string_formatter.h"
#include "atom/core/string/string_formatter_provider.h"
#include "fmt/core.h"
#include "fmt/format.h"

namespace atom
{
    constexpr auto _fmt_error_to_string_format_error(const fmt::format_error& err)
        -> string_format_error
    {
        return string_format_error("conversion from fmt::format_error to atom::string_format_error "
                                   "is not implemented yet.");
    }

    template <typename... arg_types>
    constexpr auto _convert_format_string_atom_to_fmt(format_string<arg_types...> fmt)
    {
        return fmt::runtime(fmt.str.to_std());
    }

    template <typename output_type>
    constexpr auto _wrap_output_type_atom_to_fmt(output_type* out)
    {
        class _wrapper
        {
        public:
            using valueue_type = char;
            using difference_type = std::ptrdiff_t;
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
            typeinfo::get_pure<output_type>* out;
        };

        return _wrapper(out);
    }

    template <typename output_type, typename... arg_types>
    constexpr auto _format_to(
        output_type&& out, format_string<arg_types...> fmt, arg_types&&... args)
    {
        try
        {
            fmt::format_to(_wrap_output_type_atom_to_fmt(&out),
                _convert_format_string_atom_to_fmt<arg_types...>(fmt),
                _format_arg_wrapper(atom::forward<arg_types>(args))...);
        }
        catch (const fmt::format_error& err)
        {
            throw _fmt_error_to_string_format_error(err);
        }
    }
}
