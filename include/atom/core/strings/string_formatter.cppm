export module atom.core:strings.string_formatter;

import fmt;
import :types;
import :strings.string_view;
import :strings.string_format_context;
import :strings._string_type_id;

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// level of `string_formatter` implementation, this is used to prevent ambiguities.
    /// --------------------------------------------------------------------------------------------
    export enum class string_formatter_level
    {
        atom,
        fmt,
        user
    };

    /// --------------------------------------------------------------------------------------------
    /// parses and writes string representation for types according to the specified format
    /// specifiers.
    /// --------------------------------------------------------------------------------------------
    export template <typename value_t, string_formatter_level level = string_formatter_level::user>
    class string_formatter;

    /// --------------------------------------------------------------------------------------------
    /// `string_formatter` specialization for all types for which exists a `fmt::formatter`
    /// specialization.
    /// --------------------------------------------------------------------------------------------
    export template <typename value_t>
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
    export template <typename string_t>
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
    export template <typename enum_t>
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
