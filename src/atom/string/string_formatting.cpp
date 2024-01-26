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
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
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
            : _fmt_ctx(fmt_ctx)
        {}

    public:
        constexpr auto get_range() -> string_view
        {
            return string_view(make_range(_fmt_ctx.begin(), _fmt_ctx.end()));
        }

        constexpr auto advance_to(array_iter<uchar> it)
        {
            _fmt_ctx.advance_to(it.data().as_unsafe<char>().unwrap());
        }

        constexpr auto _release_native() -> fmt::format_parse_context&
        {
            return _fmt_ctx;
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
        constexpr string_format_context(fmt::format_context& ctx)
            : _fmt_ctx(ctx)
            , _fmt_ctx_out(ctx.out())
        {}

        constexpr ~string_format_context()
        {
            _fmt_ctx.advance_to(mov(_fmt_ctx_out));
        }

    public:
        auto write(char ch)
        {
            *_fmt_ctx_out++ = ch;
        }

        template <typename range_type>
        auto write(const range_type& range)
            requires(rrange_of<range_type, uchar>)
        {
            for (uchar ch : range)
            {
                *_fmt_ctx_out++ = ch.unwrap();
            }
        }

        constexpr auto _release_native() -> fmt::format_context&
        {
            return _fmt_ctx;
        }

    public:
        fmt::format_context& _fmt_ctx;
        typename fmt::format_context::iterator _fmt_ctx_out;
    };

    /// --------------------------------------------------------------------------------------------
    /// parses and writes string representation for types according to the specified format
    /// specifiers.
    /// --------------------------------------------------------------------------------------------
    export template <typename type>
    class string_formatter;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `string formattable`.
    /// --------------------------------------------------------------------------------------------
    export template <typename type>
    concept rstring_formattable = requires(string_formatter<type> formatter, type arg,
        string_format_parse_context parse_ctx, string_format_context ctx) {
        formatter.parse(parse_ctx);
        formatter.format(arg, ctx);
    };
}

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// uses `fmt` implementation to implement `atom::string_formatter`.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    class _string_formatter_helper
    {
    public:
        constexpr auto parse(string_format_parse_context& context)
        {
            fmt::format_parse_context& native = context._release_native();
            fmt::format_parse_context::iterator begin = _formatter.parse(native);
            native.advance_to(begin);
        }

        constexpr auto format(type val, string_format_context& context)
        {
            fmt::format_context& native = context._release_native();
            fmt::format_context::iterator begin = _formatter.format(val, native);
            native.advance_to(begin);
        }

    private:
        fmt::formatter<type> _formatter;
    };

    /// --------------------------------------------------------------------------------------------
    /// `string_formatter` specialization for all numeric types.
    /// --------------------------------------------------------------------------------------------
    template <typename num_type>
        requires rnum<num_type>
    class string_formatter<num_type>
    {
    public:
        constexpr auto parse(string_format_parse_context& ctx)
        {
            _formatter.parse(ctx);
        }

        constexpr auto format(num_type num, string_format_context& ctx)
        {
            _formatter.format(num.unwrap(), ctx);
        }

    private:
        _string_formatter_helper<typename num_type::unwrapped_type> _formatter;
    };

    /// --------------------------------------------------------------------------------------------
    /// `string_formatter` specialization for `uchar`.
    /// --------------------------------------------------------------------------------------------
    template <>
    class string_formatter<uchar>
    {
    public:
        constexpr auto parse(string_format_parse_context& ctx)
        {
            _formatter.parse(ctx);
        }

        constexpr auto format(uchar ch, string_format_context& ctx)
        {
            _formatter.format(ch.unwrap(), ctx);
        }

    private:
        _string_formatter_helper<char> _formatter;
    };

    /// --------------------------------------------------------------------------------------------
    /// `string_formatter` specialization for `byte`.
    /// --------------------------------------------------------------------------------------------
    template <>
    class string_formatter<byte>
    {
    public:
        constexpr auto parse(string_format_parse_context& ctx)
        {
            _formatter.parse(ctx);
        }

        constexpr auto format(byte val, string_format_context& ctx)
        {
            _formatter.format(val, ctx);
        }

    private:
        _string_formatter_helper<byte> _formatter;
    };

    /// --------------------------------------------------------------------------------------------
    /// `string_formatter` specialization for `string_view`.
    /// --------------------------------------------------------------------------------------------
    template <>
    class string_formatter<string_view>
    {
    public:
        constexpr auto parse(string_format_parse_context& ctx)
        {
            _formatter.parse(ctx);
        }

        constexpr auto format(string_view str, string_format_context& ctx)
        {
            fmt::string_view fmt_str(str.data().as_unsafe<char>().unwrap(), str.count().unwrap());
            _formatter.format(fmt_str, ctx);
        }

    private:
        _string_formatter_helper<fmt::string_view> _formatter;
    };

    /// --------------------------------------------------------------------------------------------
    /// `string_formatter` specialization for `char` arr.
    /// --------------------------------------------------------------------------------------------
    template <usize count>
    class string_formatter<char[count]>: public string_formatter<string_view>
    {
    public:
        auto format(const char (&chars)[count], string_format_context& ctx)
        {
            string_view str(range_from_literal(chars));
            string_formatter<string_view>::format(str, ctx);
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// `string_formatter` specialization for `range`.
    /// --------------------------------------------------------------------------------------------
    template <typename range_type>
        requires rrange<range_type> and rstring_formattable<typename range_type::elem_type>
    class string_formatter<range_type>
    {
        class _range_wrapper
        {
        public:
            constexpr _range_wrapper(const range_type& range)
                : _range(range)
            {}

        public:
            constexpr auto begin()
            {
                return std_iter_wrap_for_atom_iter(_range.iter());
            }

            constexpr auto end()
            {
                return std_iter_wrap_for_atom_iter(_range.iter_end());
            }

        private:
            const range_type& _range;
        };

    public:
        constexpr auto parse(string_format_parse_context& ctx)
        {
            _formatter.parse(ctx);
        }

        constexpr auto format(const range_type& range, string_format_context& ctx)
        {
            _formatter.format(_range_wrapper(range), ctx);
        }

    private:
        _string_formatter_helper<_range_wrapper> _formatter;
    };

    template <typename type>
    class _fmt_formatter_filter
    {
    public:
        static constexpr bool enable = true;
    };

    template <usize count>
    class _fmt_formatter_filter<char[count]>
    {
    public:
        static constexpr bool enable = false;
    };
}

namespace fmt
{
    /// --------------------------------------------------------------------------------------------
    /// `fmt::formatter` specialization for all types that implement `atom::string_formatter`.
    /// `fmt` uses this type and users specialize `atom::string_formatter`.
    /// --------------------------------------------------------------------------------------------
    export template <typename type>
        requires atom::rstring_formattable<type> and atom::_fmt_formatter_filter<type>::enable
    class formatter<type>
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
            _atom_formatter.format(in, ctx);
            return fmt_ctx.out();
        }

    private:
        /// ----------------------------------------------------------------------------------------
        /// this_type contains actual implementation.
        /// ----------------------------------------------------------------------------------------
        atom::string_formatter<type> _atom_formatter;
    };
}
