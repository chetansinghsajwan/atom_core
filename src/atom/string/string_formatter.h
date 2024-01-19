#pragma once
#include "string_arg_formatters.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    class _fmt_string_view_cnvter
    {
    public:
        constexpr auto from_fmt(_fmt_string_view str) -> string_view
        {
            const uchar* begin = _from_std_char_ptr(str.data());
            usize count = str.size();
            return string_view{ make_range(begin, count) };
        }

        constexpr auto to_fmt(string_view str) -> _fmt_string_view
        {
            const char* begin = _to_std_char_ptr(str.data().unwrap());
            const _usize count = str.count().unwrap();
            return _fmt_string_view{ begin, count };
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// wrapper over {string_view} to represent format string. this_type is done to avoid compile
    /// time checks.
    /// --------------------------------------------------------------------------------------------
    class run_fmt_string
    {
    public:
        string_view str;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <rstring_fmt_arg_fmtable... arg_types>
    using fmt_string = _fmt_fmt_string<arg_types...>;

    //     class fmt_string
    //     {
    //     public:
    //         template <typename type>
    //         consteval fmt_string(const type& strv) { }
    //             _fmt{ _fmt_string_view_cnvter().to_fmt(strv) } { }
    //
    //         fmt_string(run_fmt_string str) { }
    //             _fmt{ _fmt_run_fmt_string{ _fmt_string_view_cnvter().to_fmt(str.str) } } { }
    //
    //     public:
    //         _fmt_fmt_string<arg_types...> _fmt;
    //     };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    class string_fmter
    {
    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename output_type, rstring_fmt_arg_fmtable... arg_types>
        auto fmt_to(output_type out, fmt_string<arg_types...> fmt, arg_types&&... args)
            requires routput<output_type, uchar>
        {
            class _out_iter_wrap
            {
            public:
                auto operator++(int) -> _out_iter_wrap&
                {
                    return *this;
                }

                auto operator*() -> _out_iter_wrap&
                {
                    return *this;
                }

                auto operator=(char ch) -> _out_iter_wrap&
                {
                    *out += uchar(ch);
                    return *this;
                }

            public:
                output_type* out;
            };

            fmt::detail::iterator_buffer<_out_iter_wrap, char> buf{ _out_iter_wrap{ &out } };

            try
            {
                fmt::detail::vformat_to<char>(buf, fmt,
                    fmt::make_format_args<fmt::buffer_context<char>>(args...),
                    fmt::detail::locale_ref{});
            }
            catch (const _fmt_fmt_ex& err)
            {
                throw string_fmt_err(err);
            }
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <rstring_fmt_arg_fmtable... arg_types>
        auto fmt(fmt_string<arg_types...> fmt, arg_types&&... args) -> string
        {
            string out;
            fmt_to(out, fmt, forward<arg_types>(args)...);

            return out;
        }
    };
}
