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
	requires (!RSameAs<T, TTI::TRemoveCVRef<T>>)
        and RStrConvertible<TTI::TRemoveCVRef<T>>
	class StrConverter<T>: StrConverter<TTI::TRemoveCVRef<T>> { };

    /// --------------------------------------------------------------------------------------------
    /// {StrConverter} specialization for {Str}.
    /// --------------------------------------------------------------------------------------------
    template < >
    class StrConverter<Str>
    {
        pub constexpr fn Convert(Str&& str) -> Str
        {
            return mov(str);
        }

        pub constexpr fn Convert(const Str& str) -> Str
        {
            return Str{ str };
        }

        pub constexpr fn Convert(const Str& str, ROutput<Char> auto out) -> void
        {
            out += str;
        }
    };
}