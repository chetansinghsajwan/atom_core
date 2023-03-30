#pragma once
#include "Atom/String/StringView.h"
#include "Atom/String/StringConverter.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// Converts object to {StringView}.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class StringViewConverter;
    // {
    //     static_assert(sizeof(T) == 0, "StringViewConverter for this type is not defined.");
    // };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is convertible to {StringView}.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RStringViewConvertible = requires(const T& in)
    {
        { StringViewConverter<T>::Convert(in) } -> RSameAs<StringView>;
    };

    /// --------------------------------------------------------------------------------------------
    /// {StringViewConverter} specialization for {StringView}.
    /// --------------------------------------------------------------------------------------------
    template < >
    class StringViewConverter<StringView>
    {
        static constexpr StringView Convert(StringView in) noexcept
        {
            return in;
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// {StringConverter} specialization for types which are {StringViewConvertible}.
    /// --------------------------------------------------------------------------------------------
    template <RStringViewConvertible TStringViewConvertible>
    class StringConverter <TStringViewConvertible>
    {
        static constexpr String Convert(const TStringViewConvertible& in) noexcept
        {
            return StringViewConverter<TStringViewConvertible>::Convert();
        }

        static constexpr void Convert(const TStringViewConvertible& in, 
            RBackInsertable<Char> auto& out) noexcept
        {
            StringView strView = Convert(in);
            out.InsertBack(in);
        }
    };
}