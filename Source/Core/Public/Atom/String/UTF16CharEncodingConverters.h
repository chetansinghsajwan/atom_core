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
        template <RConstIterable<UTF16Char> TIn, RBackInsertable<UTF8Char> TOut>
        static constexpr void ConvertChar(TIn& in, TIn& in_end, TOut& out)
        {
        }
    };

    template < >
    struct CharEncodingConversionImpl<UTF16CharEncoding, UTF32CharEncoding>
    {
        template <RConstIterable<UTF16Char> TIn, RBackInsertable<UTF32Char> TOut>
        static constexpr void ConvertChar(TIn& in, TIn& in_end, TOut& out)
        {
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