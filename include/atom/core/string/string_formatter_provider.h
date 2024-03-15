#pragma once
#include "atom/core/string/_format_arg_wrapper.h"
#include "atom/core/string/string_formatter.h"
#include "atom/core/typeinfo.h"
#include "fmt/core.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// finds and provides `string_formatter` implementation to use.
    /// --------------------------------------------------------------------------------------------
    template <typename in_value_t>
    class string_formatter_provider
    {
        using value_t = typeinfo::get_pure<in_value_t>;

    private:
        static consteval auto _get_t_indentity() -> decltype(auto)
        {
            using _formatter_atom = string_formatter<value_t, string_formatter_level::atom>;
            using _formatter_fmt = string_formatter<value_t, string_formatter_level::fmt>;
            using _formatter_user = string_formatter<value_t, string_formatter_level::user>;

            if constexpr (rdefault_constructible<_formatter_atom>)
            {
                return std::type_identity<_formatter_atom>();
            }
            else if constexpr (rdefault_constructible<_formatter_fmt>)
            {
                return std::type_identity<_formatter_fmt>();
            }
            else if constexpr (rdefault_constructible<_formatter_user>)
            {
                return std::type_identity<_formatter_user>();
            }
            else
            {
                return std::type_identity<void>();
            }
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// type of formatter specialization.
        /// ----------------------------------------------------------------------------------------
        using formatter_t = decltype(_get_t_indentity())::type;

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if an implementation exists.
        /// ----------------------------------------------------------------------------------------
        static consteval auto has() -> bool
        {
            if constexpr (typeinfo::is_same<formatter_t, void>)
                return false;

            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the implementation if exists, else return type is void.
        /// ----------------------------------------------------------------------------------------
        static constexpr auto get() -> decltype(auto)
            requires(has())
        {
            return formatter_t();
        }
    };
}

namespace fmt
{
    /// --------------------------------------------------------------------------------------------
    /// this `fmt::formatter` speicialization connects `fmt` with `atom::formatter` implementations.
    /// calls `atom::formatter` implementation for `value_t`.
    /// --------------------------------------------------------------------------------------------
    template <typename value_t>
    class formatter<atom::_format_arg_wrapper<value_t>>
    {
    public:
        constexpr auto parse(fmt::format_parse_context& fmt_ctx) ->
            typename fmt::format_parse_context::iterator
        {
            atom::string_format_parse_context ctx(fmt_ctx);
            _formatter.parse(ctx);
            return fmt_ctx.begin();
        }

        constexpr auto format(atom::_format_arg_wrapper<value_t>& value,
            fmt::format_context& fmt_ctx) const -> typename fmt::format_context::iterator
        {
            atom::string_format_context ctx(fmt_ctx);
            _formatter.format(value.value, ctx);
            return fmt_ctx.out();
        }

    private:
        atom::string_formatter_provider<value_t>::formatter_t _formatter;
    };
}
