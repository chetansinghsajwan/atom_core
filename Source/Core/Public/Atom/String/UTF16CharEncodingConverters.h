#pragma once
#include "Atom/String/CharEncodingConverter.h"
#include "Atom/String/UTF8CharEncoding.h"
#include "Atom/String/UTF16CharEncoding.h"
#include "Atom/String/UTF32CharEncoding.h"

namespace Atom::Private
{
    template < >
    struct CharEncodingConversionImpl<UTF16CharEncoding, UTF8CharEncoding>
    {
        template <typename TIn, typename TOut>
        requires RFwdIter<TIn, UTF16Char> && ROutput<TOut, UTF8Char>
        static constexpr void ConvertChar(TIn in, TOut out)
        {
            static constexpr UTF8StringView msg = UTF8(
                "UTF16 -> UTF8 char encoding conversion not implemented yet.");

            out += msg;
        }
    };

    template < >
    struct CharEncodingConversionImpl<UTF16CharEncoding, UTF32CharEncoding>
    {
        template <typename TIn, typename TOut>
        requires RFwdIter<TIn, UTF16Char> && ROutput<TOut, UTF32Char>
        static constexpr void ConvertChar(TIn in, TOut out)
        {
            static constexpr UTF32StringView msg = UTF32(
                "UTF16 -> UTF32 char encoding conversion not implemented yet.");

            out += msg;
        }
    };
}

namespace Atom
{
    static_assert(RCharEncodingConvertible<UTF16CharEncoding, UTF8CharEncoding>,
        "No or Invalid CharEncodingConverter<UTF16CharEncoding, UTF8CharEncoding> "
        "specialization specified.");

    static_assert(RCharEncodingConvertible<UTF16CharEncoding, UTF16CharEncoding>,
        "No or Invalid CharEncodingConverter<UTF16CharEncoding, UTF16CharEncoding> "
        "specialization specified.");

    static_assert(RCharEncodingConvertible<UTF16CharEncoding, UTF32CharEncoding>,
        "No or Invalid CharEncodingConverter<UTF16CharEncoding, UTF32CharEncoding> "
        "specialization specified.");

    static_assert(RCharEncodingLazyConvertible<UTF16CharEncoding, UTF8CharEncoding>,
        "No or Invalid CharEncodingLazyConverter<UTF16CharEncoding, UTF8CharEncoding> "
        "specialization specified.");

    static_assert(RCharEncodingLazyConvertible<UTF16CharEncoding, UTF16CharEncoding>,
        "No or Invalid CharEncodingLazyConverter<UTF16CharEncoding, UTF16CharEncoding> "
        "specialization specified.");

    static_assert(RCharEncodingLazyConvertible<UTF16CharEncoding, UTF32CharEncoding>,
        "No or Invalid CharEncodingLazyConverter<UTF16CharEncoding, UTF32CharEncoding> "
        "specialization specified.");

    using UTF16ToUTF8Converter = CharEncodingConverter<
        UTF16CharEncoding, UTF8CharEncoding>;

    using UTF16ToUTF32Converter = CharEncodingConverter<
        UTF16CharEncoding, UTF32CharEncoding>;

    template <typename TInputIt>
    using UTF16ToUTF8LazyConverter = CharEncodingLazyConverter<
        UTF16CharEncoding, UTF8CharEncoding, TInputIt>;

    template <typename TInputIt>
    using UTF16ToUTF32LazyConverter = CharEncodingLazyConverter<
        UTF16CharEncoding, UTF32CharEncoding, TInputIt>;
}