export module atom.core:string_formatting;
import :std;
import :fmt;
import :core;
import :error;
import :array_iter;
import :range;
import :string_view;

namespace atom
{
    export template <typename... arg_types>
    using format_string = fmt::format_string<arg_types...>;

    /// --------------------------------------------------------------------------------------------
    /// string formatting error.
    /// --------------------------------------------------------------------------------------------
    export class string_format_error: public error
    {
    public:
        constexpr string_format_error(std::string_view msg)
            : error(msg)
        {}
    };

    constexpr auto _fmt_error_to_string_format_error(const fmt::format_error& err)
        -> string_format_error
    {
        return string_format_error("conversion from fmt::format_error to atom::string_format_error "
                                   "is not implemented yet.");
    }

    /// --------------------------------------------------------------------------------------------
    /// context to parse format string.
    /// --------------------------------------------------------------------------------------------
    export class string_format_parse_context
    {
    public:
        constexpr string_format_parse_context(fmt::format_parse_context& fmt_ctx)
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
        fmt::format_parse_context& _fmt_ctx;
    };

    /// --------------------------------------------------------------------------------------------
    /// context to write formatted string.
    /// --------------------------------------------------------------------------------------------
    export class string_format_context
    {
    public:
        constexpr string_format_context(fmt::format_context& fmt_ctx)
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

        constexpr auto release_native() -> fmt::format_context*
        {
            return &_fmt_ctx;
        }

    public:
        fmt::format_context& _fmt_ctx;
    };

    /// --------------------------------------------------------------------------------------------
    /// parses and writes string representation for types according to the specified format
    /// specifiers.
    /// --------------------------------------------------------------------------------------------
    export template <typename type>
    class string_formatter;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is {string formattable}.
    /// --------------------------------------------------------------------------------------------
    export template <typename type>
    concept rstring_formattable = requires(string_formatter<type> formatter, type arg,
        string_format_parse_context parse_ctx, string_format_context ctx)
    {
        formatter.parse(parse_ctx);
        formatter.format(arg, ctx);
    };
}

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// {string_formatter} specialization for {string_view}.
    ///
    /// @internal
    /// uses {fmt::formatter<fmt::string_view>} specialization.
    /// --------------------------------------------------------------------------------------------
    template <>
    class string_formatter<string_view>
    {
    public:
        constexpr auto parse(string_format_parse_context& ctx)
        {
            fmt::format_parse_context& fmt_ctx = ctx._fmt_ctx;

            fmt_ctx.advance_to(_fmt_formatter.parse(fmt_ctx));
        }

        auto format(string_view str, string_format_context& ctx)
        {
            fmt::format_context& fmt_ctx = ctx._fmt_ctx;

            fmt::string_view fmt_str{ _to_std_char_ptr(str.data().unwrap()), str.count() };
            fmt_ctx.advance_to(_fmt_formatter.format(fmt_str, fmt_ctx));
        }

    public:
        fmt::formatter<fmt::string_view> _fmt_formatter;
    };

    static_assert(rstring_formattable<string_view>, "string_view is not formattable.");

    /// --------------------------------------------------------------------------------------------
    /// {string_formatter} specialization for {char} arr.
    /// --------------------------------------------------------------------------------------------
    template <usize n>
    class string_formatter<char[n]>: public string_formatter<string_view>
    {
    public:
        auto format(const char (&chars)[n], string_format_context& ctx)
        {
            string_view str(chars);
            string_formatter<string_view>::format(str, ctx);
        }
    };

    template <typename type>
    class _fmt_formatter_filter
    {
    public:
        static constexpr bool enable = true;
    };

    template <usize n>
    class _fmt_formatter_filter<char[n]>
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
    /// {fmt::formatter} specialization for all types that implement {atom::string_formatter}.
    /// {fmt} uses this type and users specialize {atom::string_formatter}.
    ///
    /// this_type is specialized for {char} character type only as {atom} uses that type for
    /// character representation.
    /// --------------------------------------------------------------------------------------------
    template <atom::rstring_formattable type>
        requires(atom::_fmt_formatter_filter<type>::enable)
    class formatter<type, atom::uchar>
    {
    public:
        constexpr auto parse(fmt::format_parse_context& fmt_ctx) ->
            typename fmt::format_parse_context::iterator
        {
            atom::string_format_parse_context ctx(fmt_ctx);
            _atom_formatter.parse(ctx);
            return fmt_ctx.begin();
        }

        constexpr auto format(const type& in, fmt::format_context& fmt_ctx) ->
            typename fmt::format_context::iterator
        {
            atom::string_format_context ctx(fmt_ctx);
            _atom_formatter.fmt(in, ctx);
            return fmt_ctx.out();
        }

    private:
        /// ----------------------------------------------------------------------------------------
        /// this_type contains actual implementation.
        /// ----------------------------------------------------------------------------------------
        atom::string_formatter<type> _atom_formatter;
    };
}
