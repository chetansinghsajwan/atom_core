#pragma once
#include "atom/core/string/string_format_core.h"
#include "atom/core/string/string_format_error.h"
#include "atom/core/string/string_formatter.h"
#include "fmt/format.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// this type is used to wrap a type, so that `fmt::formatter` specializations cannot recognize
    /// the type. so that fmt has to use our specialization.
    ///
    /// this is done to take control of the specializations and prevent ambiguity and hide
    /// implementation details.
    /// --------------------------------------------------------------------------------------------
    template <typename in_value_type>
    class _value_wrapper
    {
    public:
        using value_type = in_value_type;

    public:
        _value_wrapper(value_type& value)
            : value(value)
        {}

    public:
        value_type& value;
    };

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
                out_iter_wrap(&out), fmt, _value_wrapper(atom::forward<arg_types>(args))...);
        }
        catch (const fmt::format_error& err)
        {
            throw _fmt_error_to_string_format_error(err);
        }
    }
}

namespace fmt
{
    /// --------------------------------------------------------------------------------------------
    /// this `fmt::formatter` speicialization connects `fmt` with `atom::formatter` implementations.
    /// calls `atom::formatter` implementation for `value_type`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_type>
    class formatter<atom::_value_wrapper<value_type>>
    {
    public:
        constexpr auto parse(fmt::format_parse_context& fmt_ctx) ->
            typename fmt::format_parse_context::iterator
        {
            atom::string_format_parse_context ctx(fmt_ctx);
            _formatter.parse(ctx);
            return fmt_ctx.begin();
        }

        constexpr auto format(atom::_value_wrapper<value_type>& value,
            fmt::format_context& fmt_ctx) const -> typename fmt::format_context::iterator
        {
            atom::string_format_context ctx(fmt_ctx);
            _formatter.format(value.value, ctx);
            return fmt_ctx.out();
        }

    private:
        atom::string_formatter_provider<value_type>::formatter_type _formatter;
    };
}
