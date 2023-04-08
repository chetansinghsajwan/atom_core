#pragma once
#include "Atom/String/StringConverter.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// Converts {T} object to {StringView}.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct StringViewConverter;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TConverter} can convert {T} object to {StringView}.
    /// --------------------------------------------------------------------------------------------
    template <typename TConverter, typename T>
    concept RStringViewConverter = requires(TConverter converter)
    {
        { converter.Convert(declval(T)) } -> RSameAs<StringView>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is convertible to {StringView}.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RStringViewConvertible = RStringViewConverter<StringViewConverter<T>, T>;

    /// --------------------------------------------------------------------------------------------
    /// {StringViewConverter} specialization for {StringView}.
    /// --------------------------------------------------------------------------------------------
    template < >
    class StringViewConverter<StringView>
    {
        constexpr StringView Convert(StringView in) noexcept
        {
            return in;
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// {StirngViewConverter} specialization for {T} containing {const}, {volatile} or {lvalue} 
    /// and {rvalue} reference.
    /// 
    /// @TODO Needs refactoring.
    /// --------------------------------------------------------------------------------------------
	template <typename T>
	requires (!RSameAs<T, TTI::TRemoveCVRef<T>>) && RStringViewConvertible<TTI::TRemoveCVRef<T>>
	struct StringViewConverter<T>: StringViewConverter<TTI::TRemoveCVRef<T>> { };

    /// --------------------------------------------------------------------------------------------
    /// {StringConverter} specialization for types which are {StringViewConvertible}.
    /// --------------------------------------------------------------------------------------------
    template <RStringViewConvertible T>
    struct StringConverter<T>
    {
        constexpr String Convert(const T& in) noexcept
        {
            return converter.Convert();
        }

        constexpr void Convert(const T& in, RBackInsertable<Char> auto& out) noexcept
        {
            StringView strView = converter.Convert(in);
            out.InsertBack(in);
        }

        StringViewConverter<T> converter;
    };
}