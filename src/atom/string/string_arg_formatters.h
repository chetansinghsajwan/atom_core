#pragma once
// #include "fmt/format.h"

// #include "string.h"
#include "string_converter.h"
#include "string_view_converter.h"


// #include "atom/tti.h"

namespace atom
{
    using _fmt_fmt_parse_ctx = ::fmt::basic_format_parse_context<char>;
    using _fmt_fmt_parse_ctx_iter = typename _fmt_fmt_parse_ctx::iterator;

    using _fmt_fmt_ctx_out = ::fmt::detail::buffer_appender<char>;
    using _fmt_fmt_ctx = ::fmt::basic_format_context<_fmt_fmt_ctx_out, char>;

    template <typename... arg_types>
    using _fmt_fmt_string = ::fmt::basic_format_string<char, fmt::type_identity_t<arg_types>...>;
    using _fmt_run_fmt_string = ::fmt::runtime_format_string<char>;
    using _fmt_string_view = ::fmt::basic_string_view<char>;

    template <typename type>
    using _fmt_fmter = ::fmt::formatter<type, char>;

    using _fmt_fmt_ex = ::fmt::format_error;

    /// --------------------------------------------------------------------------------------------
    /// error thrown during formatting error.
    /// --------------------------------------------------------------------------------------------
    class string_fmt_error: public error
    {
    public:
        string_fmt_error(std::string_view msg)
            : error(msg)
        {}

    public:
        string_fmt_error(const _fmt_fmt_ex& fmt_ex)
            : error(fmt_ex.what())
        {}
    };

    /// --------------------------------------------------------------------------------------------
    /// context to parse format string.
    /// --------------------------------------------------------------------------------------------
    class string_fmt_parse_ctx
    {
    public:
        constexpr string_fmt_parse_ctx(_fmt_fmt_parse_ctx& fmt_ctx)
            : _fmt_ctx{ fmt_ctx }
        {}

    public:
        constexpr auto get_range() -> string_view
        {
            return string_view(make_range(_fmt_ctx.begin(), _fmt_ctx.end()));
        }

        constexpr auto advance_to(array_iter<uchar> it)
        {
            _fmt_ctx.advance_to(_to_std_char_ptr(it.data().unwrap()));
        }

    public:
        _fmt_fmt_parse_ctx& _fmt_ctx;
    };

    /// --------------------------------------------------------------------------------------------
    /// context to write formatted string.
    /// --------------------------------------------------------------------------------------------
    class string_fmt_ctx
    {
    public:
        constexpr string_fmt_ctx(_fmt_fmt_ctx& fmt_ctx)
            : _fmt_ctx{ fmt_ctx }
        {}

        auto write(char ch)
        {
            auto out = _fmt_ctx.out();
            *out++ = ch;
            _fmt_ctx.advance_to(mov(out));
        }

        template <typename range_type>
        auto write(const range_type& chars)
            requires(rrange_of<range_type, char>)
        {
            auto out = _fmt_ctx.out();
            for (char ch : chars)
            {
                *out++ = ch;
            }

            _fmt_ctx.advance_to(mov(out));
        }

    public:
        _fmt_fmt_ctx& _fmt_ctx;
    };

    /// --------------------------------------------------------------------------------------------
    /// parses and writes string representation for types according to the specified format
    /// specifiers.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    class string_fmt_arg_fmter_impl;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    using string_fmt_arg_fmter = string_fmt_arg_fmter_impl<tti::remove_cvref_type<type>>;

    /// --------------------------------------------------------------------------------------------
    /// ensures {tfmter} is {string_fmt_arg_fmter} for type `type`.
    /// --------------------------------------------------------------------------------------------
    template <typename tfmter, typename type>
    concept rstring_fmt_arg_fmter =
        requires(tfmter fmter, type arg, string_fmt_parse_ctx parse_ctx, string_fmt_ctx ctx) {
            fmter.parse(parse_ctx);
            fmter.fmt(arg, ctx);
        };

    /// --------------------------------------------------------------------------------------------
    /// string_arg_fmtable refers to a type for which exists a valid {string_fmt_arg_fmter<type>} specialization
    /// which satisfies {rstring_fmt_arg_fmter<string_fmt_arg_fmter<type>, type>} requirement.
    /// --------------------------------------------------------------------------------------------

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is {string_arg_fmtable}.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    concept rstring_fmt_arg_fmtable = rstring_fmt_arg_fmter<string_fmt_arg_fmter<type>, type>;
}

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// {string_fmt_arg_fmter} specialization for {string_view}.
    ///
    /// @internal
    /// uses {fmt::fmter<fmt::string_view>} specialization.
    /// --------------------------------------------------------------------------------------------
    template <>
    class string_fmt_arg_fmter_impl<string_view>
    {
    public:
        constexpr auto parse(string_fmt_parse_ctx& ctx)
        {
            _fmt_fmt_parse_ctx& fmt_ctx = ctx._fmt_ctx;

            fmt_ctx.advance_to(_fmt_fmter.parse(fmt_ctx));
        }

        auto fmt(string_view str, string_fmt_ctx& ctx)
        {
            _fmt_fmt_ctx& fmt_ctx = ctx._fmt_ctx;

            _fmt_string_view fmt_str{ _to_std_char_ptr(str.data().unwrap()), str.count() };
            fmt_ctx.advance_to(_fmt_fmter.format(fmt_str, fmt_ctx));
        }

    public:
        _fmt_fmter<_fmt_string_view> _fmt_fmter;
    };

    static_assert(rstring_fmt_arg_fmtable<string_view>, "string_view is not formattable.");

    /// --------------------------------------------------------------------------------------------
    /// {string_fmt_arg_fmter} specialization for {char} arr.
    /// --------------------------------------------------------------------------------------------
    template <usize n>
    class string_fmt_arg_fmter_impl<char[n]>: public string_fmt_arg_fmter<string_view>
    {
    public:
        auto fmt(const char (&chars)[n], string_fmt_ctx& ctx)
        {
            string_view str{ chars, n };
            string_fmt_arg_fmter<string_view>::fmt(str, ctx);
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// {string_fmt_arg_fmter} specialization for types which satisfy {rstring_view_convertible} requirement.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
        requires(rstring_view_convertible<type>)
    class string_fmt_arg_fmter_impl<type>: public string_fmt_arg_fmter<string_view>
    {
    public:
        auto fmt(const type& in, string_fmt_ctx& ctx)
        {
            string_fmt_arg_fmter<string_view>::fmt(convter.convert(in), ctx);
        }

    public:
        string_view_converter<type> convter;
    };

    template <typename type>
    class _fmt_fmtfier_filter
    {
    public:
        static constexpr bool enable = true;
    };

    template <usize n>
    class _fmt_fmtfier_filter<char[n]>
    {
    public:
        static constexpr bool enable = false;
    };
}

namespace fmt
{
    /// --------------------------------------------------------------------------------------------
    /// @internal
    ///
    /// {fmt::fmter} specialization for all types that implement {atom::string_fmt_arg_fmter}.
    /// {fmt} uses this type and users specialize {atom::string_fmt_arg_fmter}.
    ///
    /// this_type is specialized for {char} character type only as {atom} uses that type for
    /// character representation.
    /// --------------------------------------------------------------------------------------------
    template <atom::rstring_fmt_arg_fmtable type>
        requires(atom::_fmt_fmtfier_filter<type>::enable)
    class formatter<type, atom::uchar>
    {
    public:
        constexpr auto parse(atom::_fmt_fmt_parse_ctx& fmt_ctx) -> atom::_fmt_fmt_parse_ctx_iter
        {
            atom::string_fmt_parse_ctx ctx(fmt_ctx);
            this->fmter.parse(ctx);
            return fmt_ctx.begin();
        }

        constexpr auto format(const type& in, atom::_fmt_fmt_ctx& fmt_ctx) -> atom::_fmt_fmt_ctx_out
        {
            atom::string_fmt_ctx ctx(fmt_ctx);
            this->fmter.fmt(in, ctx);
            return fmt_ctx.out();
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// this_type contains actual implementation.
        /// ----------------------------------------------------------------------------------------
        atom::string_fmt_arg_fmter<type> fmter;
    };
}
