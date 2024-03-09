#pragma once
#include "atom/core/string/_format_arg_wrapper.h"
#include "atom/core/string/string_format_context.h"
#include "atom/core/string/static_string.h"
#include "atom/core/string/dynamic_string.h"
#include "atom/core/string/buf_string.h"
#include "atom/core/typeinfo.h"
#include "fmt/format.h"

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
    template <typename type, string_formatter_level level = string_formatter_level::user>
    class string_formatter;

    /// --------------------------------------------------------------------------------------------
    /// finds and provides `string_formatter` implementation to use.
    /// --------------------------------------------------------------------------------------------
    template <typename value_type>
    class string_formatter_provider
    {
        // static_assert(typeinfo::is_pure<value_type>);

    public:
        using _string_formatter_atom = string_formatter<value_type, string_formatter_level::atom>;
        using _string_formatter_fmt = string_formatter<value_type, string_formatter_level::fmt>;
        using _string_formatter_user = string_formatter<value_type, string_formatter_level::user>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// returns the implementation if exists, else return type is void.
        /// ----------------------------------------------------------------------------------------
        static constexpr auto get() -> decltype(auto)
        {
            if constexpr (rdefault_constructible<_string_formatter_atom>)
                return _string_formatter_atom();

            if constexpr (rdefault_constructible<_string_formatter_fmt>)
                return _string_formatter_fmt();

            if constexpr (rdefault_constructible<_string_formatter_user>)
                return _string_formatter_user();
        }

        /// ----------------------------------------------------------------------------------------
        /// type of formatter specialization.
        /// ----------------------------------------------------------------------------------------
        using formatter_type = decltype(get());

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if an implementation exists.
        /// ----------------------------------------------------------------------------------------
        static consteval auto has() -> bool
        {
            if constexpr (typeinfo::is_same<formatter_type, void>)
                return false;

            return true;
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// `string_formatter` specialization for all types for which exists a `fmt::formatter`
    /// specialization.
    /// --------------------------------------------------------------------------------------------
    template <typename value_type>
        requires typeinfo::is_default_constructible<fmt::formatter<value_type>>
    class string_formatter<value_type, string_formatter_level::fmt>
    {
    public:
        constexpr auto parse(string_format_parse_context& context)
        {
            fmt::format_parse_context& native = context._release_native();
            fmt::format_parse_context::iterator begin = _formatter.parse(native);
            native.advance_to(begin);
        }

        constexpr auto format(value_type value, string_format_context& context) const
        {
            fmt::format_context& native = context._release_native();
            fmt::format_context::iterator begin = _formatter.format(value, native);
            native.advance_to(begin);
        }

    private:
        fmt::formatter<value_type> _formatter;
    };

    /// --------------------------------------------------------------------------------------------
    /// `string_formatter` specialization for `string_view` to treat as string instead of range.
    /// --------------------------------------------------------------------------------------------
    template <>
    class string_formatter<string_view, string_formatter_level::atom>
        : public string_formatter<fmt::string_view, string_formatter_level::fmt>
    {
        using base_type = string_formatter<fmt::string_view, string_formatter_level::fmt>;

    public:
        constexpr auto format(string_view str, string_format_context& ctx) const -> void
        {
            fmt::string_view fmt_str(str.get_data(), str.get_count());
            base_type::format(fmt_str, ctx);
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// `string_formatter` specialization for `static_string` to treat as string instead of range.
    /// --------------------------------------------------------------------------------------------
    template <usize count>
    class string_formatter<static_string<count>, string_formatter_level::atom>
        : public string_formatter<string_view, string_formatter_level::atom>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `string_formatter` specialization for `dynamic_string` to treat as string instead of range.
    /// --------------------------------------------------------------------------------------------
    template <typename allocator_type>
    class string_formatter<dynamic_string<allocator_type>, string_formatter_level::atom>
        : public string_formatter<string_view, string_formatter_level::atom>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `string_formatter` specialization for `buf_string` to treat as string instead of range.
    /// --------------------------------------------------------------------------------------------
    template <usize count, typename allocator_type>
    class string_formatter<buf_string<count, allocator_type>, string_formatter_level::atom>
        : public string_formatter<string_view, string_formatter_level::atom>
    {};
}

namespace fmt
{
    /// --------------------------------------------------------------------------------------------
    /// this `fmt::formatter` speicialization connects `fmt` with `atom::formatter` implementations.
    /// calls `atom::formatter` implementation for `value_type`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_type>
    class formatter<atom::_format_arg_wrapper<value_type>>
    {
    public:
        constexpr auto parse(fmt::format_parse_context& fmt_ctx) ->
            typename fmt::format_parse_context::iterator
        {
            atom::string_format_parse_context ctx(fmt_ctx);
            _formatter.parse(ctx);
            return fmt_ctx.begin();
        }

        constexpr auto format(atom::_format_arg_wrapper<value_type>& value,
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
