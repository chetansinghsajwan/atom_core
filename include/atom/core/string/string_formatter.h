#pragma once
#include "atom/core/string/string_format_context.h"
// #include "atom/core/core/enums.h"
#include "atom/core/string/_string_type_id.h"
// #include "atom/core/types.h"
// #include "fmt/core.h"
// #include "fmt/ranges.h"
// #include "fmt/std.h"
// #include "fmt/chrono.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// level of `string_formatter` implementation, this is used to prevent ambiguities.
    /// --------------------------------------------------------------------------------------------
    enum class string_formatter_level
    {
        atom,
        fmt,
        user
    };

    /// --------------------------------------------------------------------------------------------
    /// parses and writes string representation for types according to the specified format
    /// specifiers.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t, string_formatter_level level = string_formatter_level::user>
    class string_formatter;

    /// --------------------------------------------------------------------------------------------
    /// `string_formatter` specialization for all types for which exists a `fmt::formatter`
    /// specialization.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
        requires typeinfo<fmt::formatter<value_t>>::is_default_constructible
    class string_formatter<value_t, string_formatter_level::fmt>
    {
    public:
        constexpr auto parse(string_format_parse_context& context)
        {
            fmt::format_parse_context& native = context._release_native();
            fmt::format_parse_context::iterator begin = _formatter.parse(native);
            native.advance_to(begin);
        }

        constexpr auto format(const value_t& value, string_format_context& context) const
        {
            fmt::format_context& native = context._release_native();
            fmt::format_context::iterator begin = _formatter.format(value, native);
            native.advance_to(begin);
        }

    private:
        fmt::formatter<value_t> _formatter;
    };

    /// --------------------------------------------------------------------------------------------
    /// `string_formatter` specialization for all string types to treat them as string instead of
    /// range of `char`.
    /// --------------------------------------------------------------------------------------------
    template <typename string_t>
        requires typeinfo<string_t>::template is_derived_from<_string_type_id>
    class string_formatter<string_t, string_formatter_level::atom>
        : public string_formatter<fmt::string_view, string_formatter_level::fmt>
    {
        using base_t = string_formatter<fmt::string_view, string_formatter_level::fmt>;

    public:
        constexpr auto format(const string_t& str, string_format_context& ctx) const -> void
        {
            fmt::string_view fmt_str(str.get_data(), str.get_count());
            base_t::format(fmt_str, ctx);
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// `string_formatter` specialization for all enum types.
    /// --------------------------------------------------------------------------------------------
    template <typename enum_t>
        requires typeinfo<enum_t>::is_enum
    class string_formatter<enum_t, string_formatter_level::atom>
        : public string_formatter<string_view, string_formatter_level::atom>
    {
        using base_t = string_formatter<string_view, string_formatter_level::atom>;

    public:
        constexpr auto format(enum_t value, string_format_context& ctx) const -> void
        {
            // base_t::format(enums::to_string_view(value), ctx);
        }
    };
}
