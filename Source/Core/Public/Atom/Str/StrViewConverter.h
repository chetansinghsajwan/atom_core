#pragma once
#include "StrView.h"
#include "StrConverter.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// Converts {T} object to {StrView}.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    struct StrViewConverter;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TConverter} can convert {T} object to {StrView}.
    /// --------------------------------------------------------------------------------------------
    template <tname TConverter, tname T>
    concept RStrViewConverter = requires(TConverter converter)
    {
        { converter.Convert(declval(T)) } -> RSameAs<StrView>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is convertible to {StrView}.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    concept RStrViewConvertible = RStrViewConverter<StrViewConverter<T>, T>;

    /// --------------------------------------------------------------------------------------------
    /// {StrViewConverter} specialization for {StrView}.
    /// --------------------------------------------------------------------------------------------
    template < >
    class StrViewConverter<StrView>
    {
        cexpr StrView Convert(StrView in) noex
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
	template <tname T>
	requires (!RSameAs<T, TTI::TRemoveCVRef<T>>) && RStrViewConvertible<TTI::TRemoveCVRef<T>>
	struct StrViewConverter<T>: StrViewConverter<TTI::TRemoveCVRef<T>> { };

    /// --------------------------------------------------------------------------------------------
    /// {StrConverter} specialization for types which are {StrViewConvertible}.
    /// --------------------------------------------------------------------------------------------
    template <RStrViewConvertible T>
    struct StrConverter<T>
    {
        cexpr Str Convert(const T& in) noex
        {
            return converter.Convert();
        }

        cexpr void Convert(const T& in, ROutput<Char> auto out) noex
        {
            StrView strView = converter.Convert(in);
            out.InsertBack(in);
        }

        StrViewConverter<T> converter;
    };
}