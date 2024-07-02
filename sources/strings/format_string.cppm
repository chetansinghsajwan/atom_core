export module atom_core:strings.format_string;

import fmt;
import :types;
import :strings._format_arg_wrapper;
import :strings.string_view;

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// type used to bypass compile time string formatting checks.
    /// --------------------------------------------------------------------------------------------
    export class runtime_format_string
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
    template <typename... arg_types>
    class _format_string
    {
    public:
        template <typename string_type>
        consteval _format_string(const string_type& str)
            requires(type_info<string_view>::is_constructible_from<string_type>())
            : str{ str }
        {
            using fmt_format_string = fmt::format_string<
                _format_arg_wrapper<typename type_info<arg_types>::pure_type::value_type>...>;

            fmt_format_string check(str);
        }

        constexpr _format_string(runtime_format_string str)
            : str{ str.str }
        {}

    public:
        string_view str;
    };

    export template <typename... arg_types>
    using format_string = _format_string<type_utils::identity_type<arg_types>...>;
}
