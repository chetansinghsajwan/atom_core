#pragma once
#include "string_converter.h"
#include "string_view.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// converts `type` object to {string_view}.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    class string_view_converter;

    /// --------------------------------------------------------------------------------------------
    /// ensures {tconverter} can convert `type` object to {string_view}.
    /// --------------------------------------------------------------------------------------------
    template <typename tconverter, typename type>
    concept rstring_view_converter = requires(tconverter converter, type arg) {
        {
            converter.convert(arg)
        } -> rsame_as<string_view>;
    };

    /// --------------------------------------------------------------------------------------------
    /// ensures `type` is convertible to {string_view}.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    concept rstring_view_convertible = rstring_view_converter<string_view_converter<type>, type>;

    /// --------------------------------------------------------------------------------------------
    /// {string_view_converter} specialization for {string_view}.
    /// --------------------------------------------------------------------------------------------
    template <>
    class string_view_converter<string_view>
    {
        constexpr string_view convert(string_view in)
        {
            return in;
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// {stirng_view_converter} specialization for `type` containing {const}, {volatile} or {lvalue}
    /// and {rvalue} reference.
    ///
    /// @todo needs refactoring.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
        requires(!rsame_as<type, tti::tremove_cvref<type>>) && rstring_view_convertible<tti::tremove_cvref<type>>
    class string_view_converter<type>: string_view_converter<tti::tremove_cvref<type>>
    {};

    /// --------------------------------------------------------------------------------------------
    /// {string_converter} specialization for types which are {string_view_convertible}.
    /// --------------------------------------------------------------------------------------------
    template <rstring_view_convertible type>
    class string_converter<type>
    {
    public:
        constexpr auto convert(const type& in) -> string
        {
            return converter.convert();
        }

        constexpr auto convert(const type& in, routput<uchar> auto out) -> void
        {
            string_view str_view = converter.convert(in);
            out.insert_back(in);
        }

    public:
        string_view_converter<type> converter;
    };
}
