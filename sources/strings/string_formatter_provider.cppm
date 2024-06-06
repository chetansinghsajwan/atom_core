export module atom.core:strings.string_formatter_provider;

import fmt;
import :types;
import :strings.string_formatter;
import :strings.string_format_context;
import :strings._format_arg_wrapper;

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// finds and provides `string_formatter` implementation to use.
    /// --------------------------------------------------------------------------------------------
    export template <typename in_value_type>
    class string_formatter_provider
    {
        using value_type = typeinfo<in_value_type>::pure_type::value_type;

    private:
        static consteval auto _get_type() -> decltype(auto)
        {
            using _formatter_atom = string_formatter<value_type, string_formatter_level::atom>;
            using _formatter_fmt = string_formatter<value_type, string_formatter_level::fmt>;
            using _formatter_user = string_formatter<value_type, string_formatter_level::user>;

            using _formatter_atom_typeinfo = typeinfo<_formatter_atom>;
            using _formatter_fmt_typeinfo = typeinfo<_formatter_fmt>;
            using _formatter_user_typeinfo = typeinfo<_formatter_user>;

            if constexpr (_formatter_atom_typeinfo::is_complete)
            {
                return _formatter_atom_typeinfo();
            }
            else if constexpr (_formatter_fmt_typeinfo::is_complete)
            {
                return _formatter_fmt_typeinfo();
            }
            else if constexpr (_formatter_user_typeinfo::is_complete)
            {
                return _formatter_user_typeinfo();
            }
            else
            {
                return typeinfo<void>();
            }
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// type of formatter specialization.
        /// ----------------------------------------------------------------------------------------
        using formatter_type = decltype(_get_type())::value_type;

        /// ----------------------------------------------------------------------------------------
        /// returns `true` if an implementation exists.
        /// ----------------------------------------------------------------------------------------
        static consteval auto has() -> bool
        {
            if constexpr (typeinfo<formatter_type>::is_void)
                return false;

            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// returns the implementation if exists, else return type is void.
        /// ----------------------------------------------------------------------------------------
        static constexpr auto get() -> decltype(auto)
            requires(has())
        {
            return formatter_type();
        }
    };
}

namespace fmt
{
    /// --------------------------------------------------------------------------------------------
    /// this `fmt::formatter` speicialization connects `fmt` with `atom::formatter` implementations.
    /// calls `atom::formatter` implementation for `value_type`.
    /// --------------------------------------------------------------------------------------------
    export template <typename value_type>
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
