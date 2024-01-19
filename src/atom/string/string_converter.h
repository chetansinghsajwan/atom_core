#pragma once
#include "atom/containers/output_req_mock.h"
#include "atom/tti.h"

#include "atom/string/string.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// converts `type` object to {string}.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    class string_converter;

    /// --------------------------------------------------------------------------------------------
    /// ensures {tstring_converter} can convert `type` object to {string}.
    ///
    /// @tparam[in] tconverter converter type to convert to stirng.
    /// @tparam[in] type object type to convert to stirng.
    /// --------------------------------------------------------------------------------------------
    template <typename tstring_converter, typename type>
    concept rstring_converter =
        requires(tstring_converter converter, type obj, output_req_mock<uchar> out) {
            {
                converter.convert(obj)
            } -> rsame_as<string>;

            {
                converter.convert(obj, out)
            } -> rsame_as<void>;
        };

    /// --------------------------------------------------------------------------------------------
    /// ensures {string_converter<type>} for `type` is {rstring_converter}.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
    concept rstring_convertible = rstring_converter<string_converter<type>, type>;

    /// --------------------------------------------------------------------------------------------
    /// {stirng_converter} specialization for `type` containing {const}, {volatile} or {lvalue}
    /// and {rvalue} reference.
    ///
    /// @todo needs refactoring.
    /// --------------------------------------------------------------------------------------------
    template <typename type>
        requires(!rsame_as<type, tti::remove_cvref_type<type>>) and rstring_convertible<tti::remove_cvref_type<type>>
    class string_converter<type>: string_converter<tti::remove_cvref_type<type>>
    {};

    /// --------------------------------------------------------------------------------------------
    /// {string_converter} specialization for {string}.
    /// --------------------------------------------------------------------------------------------
    template <>
    class string_converter<string>
    {
    public:
        constexpr auto convert(string&& str) -> string
        {
            return mov(str);
        }

        constexpr auto convert(const string& str) -> string
        {
            return string{ str };
        }

        constexpr auto convert(const string& str, routput<uchar> auto out) -> void
        {
            out += str;
        }
    };
}
