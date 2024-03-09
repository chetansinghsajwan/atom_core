#pragma once
#include "atom/core/range/array_iter.h"
#include "atom/core/string/string_view.h"
#include "fmt/core.h"

namespace atom
{
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

        constexpr auto advance_to(array_iter<char> it) -> void
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
}
