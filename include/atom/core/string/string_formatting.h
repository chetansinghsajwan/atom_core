#pragma once
#include "atom/core/_std.h"
#include "atom/core/_fmt.h"
#include "atom/core/core.h"
#include "atom/core/core/error.h"
#include "atom/core/containers/output_requirements.h"
#include "atom/core/range/array_iter.h"
#include "atom/core/range.h"
#include "atom/core/string/string_view.h"
#include "atom/core/string/static_string.h"
#include "atom/core/string/dynamic_string.h"
#include "atom/core/string/buf_string.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename... arg_types>
    using format_string = fmt::format_string<arg_types...>;

    /// --------------------------------------------------------------------------------------------
    /// string formatting error.
    /// --------------------------------------------------------------------------------------------
    class string_format_error: public error
    {
    public:
        constexpr string_format_error(string_view msg)
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
    class string_format_parse_context
    {
    public:
        constexpr string_format_parse_context(fmt::format_parse_context& fmt_ctx)
            : _fmt_ctx(fmt_ctx)
        {}

    public:
        constexpr auto get_range() -> string_view
        {
            return string_view(range_from(_fmt_ctx.begin(), _fmt_ctx.end()));
        }

        constexpr auto advance_to(array_iter<char> it)
        {
            _fmt_ctx.advance_to(it.get_data());
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
    class string_format_context
    {
    public:
        constexpr string_format_context(fmt::format_context& ctx)
            : _fmt_ctx(&ctx)
            , _fmt_ctx_out(ctx.out())
        {}

        constexpr ~string_format_context()
        {
            if (_fmt_ctx != nullptr)
                _fmt_ctx->advance_to(atom::move(_fmt_ctx_out));
        }

    public:
        auto write(char ch)
        {
            *(*_fmt_ctx_out)++ = ch;
        }

        template <typename range_type>
        auto write(const range_type& range)
            requires(rrange_of<range_type, char>)
        {
            for (char ch : range)
            {
                *(*_fmt_ctx_out)++ = ch;
            }
        }

        constexpr auto _release_native() -> fmt::format_context&
        {
            auto tmp = _fmt_ctx;
            _fmt_ctx = nullptr;
            return *tmp;
        }

    public:
        fmt::format_context* _fmt_ctx;
        typename fmt::format_context::iterator _fmt_ctx_out;
    };

    /// --------------------------------------------------------------------------------------------
    /// parses and writes string representation for types according to the specified format
    /// specifiers.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    class string_formatter;

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is `string formattable`.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    concept rstring_formattable = requires(string_formatter<type> formatter, type arg,
        string_format_parse_context parse_ctx, string_format_context ctx) {
        formatter.parse(parse_ctx);
        formatter.format(arg, ctx);
    };

    /// ----------------------------------------------------------------------------------------
    ///
    /// ----------------------------------------------------------------------------------------
    template <typename output_type, typename... arg_types>
    constexpr auto _format_to(
        output_type&& out, format_string<arg_types...> fmt, arg_types&&... args)
    // requires routput<output_type, char>
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
            pure_type<output_type>* out;
        };

        try
        {
            fmt::format_to(out_iter_wrap(&out), fmt, atom::forward<arg_types>(args)...);
        }
        catch (const fmt::format_error& err)
        {
            throw _fmt_error_to_string_format_error(err);
        }
    }
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

        constexpr auto format(type value, string_format_context& context) const
        {
            fmt::format_context& native = context._release_native();
            fmt::format_context::iterator begin = _formatter.format(value, native);
            native.advance_to(begin);
        }

    private:
        fmt::formatter<type> _formatter;
    };

    /// --------------------------------------------------------------------------------------------
    /// `string_formatter` specialization for `string_view` to treat as string instead of range.
    /// --------------------------------------------------------------------------------------------
    template <>
    class string_formatter<string_view>: public _string_formatter_helper<fmt::string_view>
    {
    public:
        constexpr auto format(string_view str, string_format_context& ctx) const
        {
            fmt::string_view fmt_str(str.get_data(), str.get_count());
            _string_formatter_helper<fmt::string_view>::format(fmt_str, ctx);
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// `string_formatter` specialization for `static_string` to treat as string instead of range.
    /// --------------------------------------------------------------------------------------------
    template <usize count>
    class string_formatter<static_string<count>>: public string_formatter<string_view>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `string_formatter` specialization for `dynamic_string` to treat as string instead of range.
    /// --------------------------------------------------------------------------------------------
    template <typename allocator_type>
    class string_formatter<dynamic_string<allocator_type>>: public string_formatter<string_view>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `string_formatter` specialization for `buf_string` to treat as string instead of range.
    /// --------------------------------------------------------------------------------------------
    template <usize count, typename allocator_type>
    class string_formatter<buf_string<count, allocator_type>>: public string_formatter<string_view>
    {};
}

namespace fmt
{
    /// --------------------------------------------------------------------------------------------
    /// helper class which calls atom::string_formatter implementation for type `type`.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    class _formatter_helper
    {
    public:
        constexpr auto parse(fmt::format_parse_context& fmt_ctx) ->
            typename fmt::format_parse_context::iterator
        {
            atom::string_format_parse_context ctx(fmt_ctx);
            _atom_formatter.parse(ctx);
            return fmt_ctx.begin();
        }

        constexpr auto format(const type& value, fmt::format_context& fmt_ctx) const ->
            typename fmt::format_context::iterator
        {
            atom::string_format_context ctx(fmt_ctx);
            _atom_formatter.format(value, ctx);
            return fmt_ctx.out();
        }

    private:
        atom::string_formatter<type> _atom_formatter;
    };

    /// --------------------------------------------------------------------------------------------
    /// `fmt::formatter` specialization for all `rstring_formattable` types.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
        requires atom::rstring_formattable<type>
    class formatter<type>: public _formatter_helper<type>
    {};
}
