#pragma once
#include "atom/core/string/_format_arg_wrapper.h"
#include "atom/core/string/string_view.h"
#include "atom/core/typeinfo.h"
#include "fmt/core.h"

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
    ///
    /// @todo implement checks.
    /// --------------------------------------------------------------------------------------------
    template <typename... arg_types>
    class _format_string
    {
    public:
        template <usize count>
        consteval _format_string(const char (&str)[count])
            : str(str)
        {}

        constexpr _format_string(runtime_format_string str)
            : str(str.str)
        {}

    public:
        string_view str;
    };

    template <typename... arg_types>
    using format_string = _format_string<typeinfo::identity_type<arg_types>...>;
}
