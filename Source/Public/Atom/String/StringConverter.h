#pragma once
#include "Atom/Containers/OutputReqMock.h"
#include "Atom/TTI.h"

#include "Atom/String/String.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// Converts `T` object to {String}.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class StringConverter;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TStringConverter} can convert `T` object to {String}.
    ///
    /// @TPARAM[IN] TConverter Converter type to convert to stirng.
    /// @TPARAM[IN] T Object type to convert to stirng.
    /// --------------------------------------------------------------------------------------------
    template <typename TStringConverter, typename T>
    concept RStringConverter = requires(TStringConverter converter, T obj, OutputReqMock<Char> out) {
        {
            converter.Convert(obj)
        } -> RSameAs<String>;

        {
            converter.Convert(obj, out)
        } -> RSameAs<void>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {StringConverter<T>} for `T` is {RStringConverter}.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RStringConvertible = RStringConverter<StringConverter<T>, T>;

    /// --------------------------------------------------------------------------------------------
    /// {StirngConverter} specialization for `T` containing {const}, {volatile} or {lvalue}
    /// and {rvalue} reference.
    ///
    /// @TODO Needs refactoring.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
        requires(!RSameAs<T, TTI::TRemoveCVRef<T>>) and RStringConvertible<TTI::TRemoveCVRef<T>>
    class StringConverter<T>: StringConverter<TTI::TRemoveCVRef<T>>
    {};

    /// --------------------------------------------------------------------------------------------
    /// {StringConverter} specialization for {String}.
    /// --------------------------------------------------------------------------------------------
    template <>
    class StringConverter<String>
    {
    public:
        constexpr auto Convert(String&& str) -> String
        {
            return mov(str);
        }

        constexpr auto Convert(const String& str) -> String
        {
            return String{ str };
        }

        constexpr auto Convert(const String& str, ROutput<Char> auto out) -> void
        {
            out += str;
        }
    };
}
