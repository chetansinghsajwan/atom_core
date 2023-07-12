#pragma once
#include "Atom/TTI.h"
#include "Atom/Containers/OutputReqMock.h"

#include "Atom/Str/Str.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// Converts {T} object to {Str}.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class StrConverter;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TStrConverter} can convert {T} object to {Str}.
    /// 
    /// @TPARAM[IN] TConverter Converter type to convert to stirng.
    /// @TPARAM[IN] T Object type to convert to stirng.
    /// --------------------------------------------------------------------------------------------
    template <typename TStrConverter, typename T>
    concept RStrConverter = requires(TStrConverter converter, T obj, OutputReqMock<Char> out)
    {
        { converter.Convert(obj) }
            -> RSameAs<Str>;

        { converter.Convert(obj, out) }
            -> RSameAs<void>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {StrConverter<T>} for {T} is {RStrConverter}.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RStrConvertible = RStrConverter<StrConverter<T>, T>;

    /// --------------------------------------------------------------------------------------------
    /// {StirngConverter} specialization for {T} containing {const}, {volatile} or {lvalue} 
    /// and {rvalue} reference.
    /// 
    /// @TODO Needs refactoring.
    /// --------------------------------------------------------------------------------------------
	template <typename T>
	requires (!RSameAs<T, TTI::TRemoveCVRef<T>>) && RStrConvertible<TTI::TRemoveCVRef<T>>
	struct StrConverter<T>: StrConverter<TTI::TRemoveCVRef<T>> { };

    /// --------------------------------------------------------------------------------------------
    /// {StrConverter} specialization for {Str}.
    /// --------------------------------------------------------------------------------------------
    template < >
    struct StrConverter<Str>
    {
        constexpr Str Convert(Str&& str) noex
        {
            return MOVE(str);
        }

        constexpr Str Convert(const Str& str) noex
        {
            return Str{ str };
        }

        constexpr void Convert(const Str& str, ROutput<Char> auto out) noex
        {
            out += str;
        }
    };
}