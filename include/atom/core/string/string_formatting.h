#pragma once
#include "atom/core/string/_format_arg_wrapper.h"
#include "atom/core/string/format_string.h"
#include "atom/core/string/string_format_error.h"
#include "atom/core/string/string_formatter.h"
#include "fmt/format.h"

namespace atom
{
    constexpr auto _fmt_error_to_string_format_error(const fmt::format_error& err)
        -> string_format_error
    {
        return string_format_error("conversion from fmt::format_error to atom::string_format_error "
                                   "is not implemented yet.");
    }

    template <typename output_type, typename... arg_types>
    constexpr auto _format_to(
        output_type&& out, format_string<arg_types...> fmt, arg_types&&... args)
    {
        class out_iter_wrap
        {
        public:
            using valueue_type = char;
            using difference_type = std::ptrdiff_t;
            using pointer = char*;
            using reference = char&;
            using iterator_category = std::output_iterator_tag;

        public:
            constexpr auto operator++() -> out_iter_wrap&
            {
                return *this;
            }

            constexpr auto operator++(int) -> out_iter_wrap&
            {
                return *this;
            }

            constexpr auto operator*() -> out_iter_wrap&
            {
                return *this;
            }

            constexpr auto operator=(char ch) -> out_iter_wrap&
            {
                *out += char(ch);
                return *this;
            }

        public:
            typeinfo::get_pure<output_type>* out;
        };

        try
        {
            fmt::format_to(
                out_iter_wrap(&out), fmt, _format_arg_wrapper(atom::forward<arg_types>(args))...);
        }
        catch (const fmt::format_error& err)
        {
            throw _fmt_error_to_string_format_error(err);
        }
    }
}
