#pragma once
#include "Atom/String/CharEncodingConverter.h"
#include "Atom/String/ASCIICharEncoding.h"
#include "Atom/String/UTF8CharEncoding.h"
#include "Atom/String/UTF16CharEncoding.h"
#include "Atom/String/UTF32CharEncoding.h"

namespace Atom::Private
{
    template < >
    struct CharEncodingConversionImpl<ASCIICharEncoding, UTF8CharEncoding>
    {
        template <typename TIn, typename TOut>
        requires RFwdRange<TIn, ASCIIChar> && ROutput<TOut, UTF8Char>
        static constexpr void ConvertChar(TIn in, TOut out)
        {
            static constexpr UTF8StringView msg = UTF8(
                "ASCII -> UTF8 char encoding conversion not implemented yet.");

            out += msg;
        }
    };

    template < >
    struct CharEncodingConversionImpl<ASCIICharEncoding, UTF16CharEncoding>
    {
        template <typename TIn, typename TOut>
        requires RFwdRange<TIn, ASCIIChar> && ROutput<TOut, UTF16Char>
        static constexpr void ConvertChar(TIn in, TOut out)
        {
            static constexpr UTF16StringView msg = UTF16(
                "ASCII -> UTF16 char encoding conversion not implemented yet.");

            out += msg;
        }
    };

    template < >
    struct CharEncodingConversionImpl<ASCIICharEncoding, UTF32CharEncoding>
    {
        template <typename TIn, typename TOut>
        requires RFwdRange<TIn, ASCIIChar> && ROutput<TOut, UTF32Char>
        static constexpr void ConvertChar(TIn in, TOut out)
        {
            static constexpr UTF32StringView msg = UTF32(
                "ASCII -> UTF32 char encoding conversion not implemented yet.");

            out += msg;
        }
    };
}

namespace Atom
{
    static_assert(RCharEncodingConvertible<ASCIICharEncoding, ASCIICharEncoding>,
        "No CharEncodingConverter<ASCIICharEncoding, ASCIICharEncoding> specialization specified.");

    static_assert(RCharEncodingConvertible<ASCIICharEncoding, UTF8CharEncoding>,
        "No CharEncodingConverter<ASCIICharEncoding, UTF8CharEncoding> specialization specified.");

    static_assert(RCharEncodingConvertible<ASCIICharEncoding, UTF16CharEncoding>,
        "No CharEncodingConverter<ASCIICharEncoding, UTF16CharEncoding> specialization specified.");

    static_assert(RCharEncodingConvertible<ASCIICharEncoding, UTF32CharEncoding>,
        "No CharEncodingConverter<ASCIICharEncoding, UTF32CharEncoding> specialization specified.");

    static_assert(RCharEncodingLazyConvertible<ASCIICharEncoding, ASCIICharEncoding>,
        "No CharEncodingLazyConverter<ASCIICharEncoding, ASCIICharEncoding> specialization specified.");

    static_assert(RCharEncodingLazyConvertible<ASCIICharEncoding, UTF8CharEncoding>,
        "No CharEncodingLazyConverter<ASCIICharEncoding, UTF8CharEncoding> specialization specified.");

    static_assert(RCharEncodingLazyConvertible<ASCIICharEncoding, UTF16CharEncoding>,
        "No CharEncodingLazyConverter<ASCIICharEncoding, UTF16CharEncoding> specialization specified.");

    static_assert(RCharEncodingLazyConvertible<ASCIICharEncoding, UTF32CharEncoding>,
        "No CharEncodingLazyConverter<ASCIICharEncoding, UTF32CharEncoding> specialization specified.");

    using ASCIIToUTF8Converter = CharEncodingConverter<
        ASCIICharEncoding, UTF8CharEncoding>;

    using ASCIIToUTF16Converter = CharEncodingConverter<
        ASCIICharEncoding, UTF16CharEncoding>;

    using ASCIIToUTF32Converter = CharEncodingConverter<
        ASCIICharEncoding, UTF32CharEncoding>;

    template <typename TInput>
    using ASCIIToUTF8LazyConverter = CharEncodingLazyConverter<
        ASCIICharEncoding, UTF8CharEncoding, TInput>;

    template <typename TInput>
    using ASCIIToUTF16LazyConverter = CharEncodingLazyConverter<
        ASCIICharEncoding, UTF16CharEncoding, TInput>;

    template <typename TInput>
    using ASCIIToUTF32LazyConverter = CharEncodingLazyConverter<
        ASCIICharEncoding, UTF32CharEncoding, TInput>;
}
