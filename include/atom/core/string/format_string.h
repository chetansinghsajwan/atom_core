#pragma once
#include "atom/core/string/_format_arg_wrapper.h"
#include "atom/core/typeinfo.h"
#include "fmt/core.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// string type used to store format string. this also checks at compile time for format or
    /// args.
    /// --------------------------------------------------------------------------------------------
    // template <typename... arg_types>
    // class _format_string
    // {
    //     using this_type = _format_string;

    // public:
    //     constexpr _format_string() = default;

    //     constexpr _format_string(const this_type& that) = default;
    //     constexpr _format_string& operator=(const this_type& that) = default;

    //     constexpr _format_string(this_type&& that) = default;
    //     constexpr _format_string& operator=(this_type&& that) = default;

    //     template <typename string_type>
    //     consteval _format_string(string_type&& str)
    //         : _str("")
    //     {}

    //     constexpr ~_format_string() = default;

    // public:
    //     fmt::format_string<_format_arg_wrapper<arg_types>...> _str;
    // };

    // template <typename... arg_types>
    // using format_string = _format_string<typeinfo::identity_type<arg_types>...>;

    template <typename... arg_types>
    using format_string = fmt::format_string<_format_arg_wrapper<arg_types>...>;
}
