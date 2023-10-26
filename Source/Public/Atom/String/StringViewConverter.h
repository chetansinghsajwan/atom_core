#pragma once
#include "StringConverter.h"
#include "StringView.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// Converts `T` object to {StringView}.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class StringViewConverter;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TConverter} can convert `T` object to {StringView}.
    /// --------------------------------------------------------------------------------------------
    template <typename TConverter, typename T>
    concept RStringViewConverter = requires(TConverter converter, T arg) {
        {
            converter.Convert(arg)
        } -> RSameAs<StringView>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures `T` is convertible to {StringView}.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RStringViewConvertible = RStringViewConverter<StringViewConverter<T>, T>;

    /// --------------------------------------------------------------------------------------------
    /// {StringViewConverter} specialization for {StringView}.
    /// --------------------------------------------------------------------------------------------
    template <>
    class StringViewConverter<StringView>
    {
        constexpr StringView Convert(StringView in)
        {
            return in;
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// {StirngViewConverter} specialization for `T` containing {const}, {volatile} or {lvalue}
    /// and {rvalue} reference.
    ///
    /// @TODO Needs refactoring.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
        requires(!RSameAs<T, TTI::TRemoveCVRef<T>>) && RStringViewConvertible<TTI::TRemoveCVRef<T>>
    class StringViewConverter<T>: StringViewConverter<TTI::TRemoveCVRef<T>>
    {};

    /// --------------------------------------------------------------------------------------------
    /// {StringConverter} specialization for types which are {StringViewConvertible}.
    /// --------------------------------------------------------------------------------------------
    template <RStringViewConvertible T>
    class StringConverter<T>
    {
    public:
        constexpr auto Convert(const T& in) -> String
        {
            return converter.Convert();
        }

        constexpr auto Convert(const T& in, ROutput<Char> auto out) -> void
        {
            StringView strView = converter.Convert(in);
            out.InsertBack(in);
        }

    public:
        StringViewConverter<T> converter;
    };
}
