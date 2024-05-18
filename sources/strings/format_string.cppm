export module atom.core:strings.format_string;

import fmt;
import :types;
import :strings._format_arg_wrapper;
import :strings.string_view;

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// type used to bypass compile time string formatting checks.
    /// --------------------------------------------------------------------------------------------
    class runtime_format_string
    {
    public:
        constexpr runtime_format_string(string_view str)
            : str(str)
        {}

    public:
        string_view str;
    };

    /// --------------------------------------------------------------------------------------------
    /// string type used to store the format for formatting. this also checks at compile time for
    /// invalid format or args.
    /// --------------------------------------------------------------------------------------------
    template <typename... arg_ts>
    class _format_string
    {
    public:
        template <typename string_t>
        consteval _format_string(const string_t& str)
            requires typeinfo<string_view>::is_constructible_from<string_t>
            : str(str)
        {
            using fmt_format_string =
                fmt::format_string<_format_arg_wrapper<typename typeinfo<arg_ts>::pure_t::value_t>...>;

            fmt_format_string check(str);
        }

        constexpr _format_string(runtime_format_string str)
            : str(str.str)
        {}

    public:
        string_view str;
    };

    template <typename... arg_ts>
    using format_string = _format_string<typeutils::identity_t<arg_ts>...>;
}
