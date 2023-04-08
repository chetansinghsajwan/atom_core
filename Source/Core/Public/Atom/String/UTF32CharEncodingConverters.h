#pragma once
#include "Atom/String/CharEncodingConverter.h"
#include "Atom/String/UTF8CharEncoding.h"
#include "Atom/String/UTF16CharEncoding.h"
#include "Atom/String/UTF32CharEncoding.h"

namespace Atom::Private
{
    template < >
    struct CharEncodingConversionImpl<UTF32CharEncoding, UTF8CharEncoding>
    {
        template <RConstIterable<UTF32Char> TIn, RBackInsertable<UTF8Char> TOut>
        static constexpr void ConvertChar(TIn& in, TIn& in_end, TOut& out)
        {
        }
    };

    template < >
    struct CharEncodingConversionImpl<UTF32CharEncoding, UTF16CharEncoding>
    {
        template <RConstIterable<UTF32Char> TIn, RBackInsertable<UTF16Char> TOut>
        static constexpr void ConvertChar(TIn& in, TIn& in_end, TOut& out)
        {
        }
    };
}

namespace Atom
{
    static_assert(RCharEncodingConvertible<UTF32CharEncoding, UTF8CharEncoding>,
        "No CharEncodingConverter<UTF32CharEncoding, UTF8CharEncoding> specialization specified.");

    static_assert(RCharEncodingConvertible<UTF32CharEncoding, UTF16CharEncoding>,
        "No CharEncodingConverter<UTF32CharEncoding, UTF16CharEncoding> specialization specified.");

    static_assert(RCharEncodingConvertible<UTF32CharEncoding, UTF32CharEncoding>,
        "No CharEncodingConverter<UTF32CharEncoding, UTF32CharEncoding> specialization specified.");

    static_assert(RCharEncodingLazyConvertible<UTF32CharEncoding, UTF8CharEncoding>,
        "No CharEncodingLazyConverter<UTF32CharEncoding, UTF8CharEncoding> specialization specified.");

    static_assert(RCharEncodingLazyConvertible<UTF32CharEncoding, UTF16CharEncoding>,
        "No CharEncodingLazyConverter<UTF32CharEncoding, UTF16CharEncoding> specialization specified.");

    static_assert(RCharEncodingLazyConvertible<UTF32CharEncoding, UTF32CharEncoding>,
        "No CharEncodingLazyConverter<UTF32CharEncoding, UTF32CharEncoding> specialization specified.");

    using UTF32ToUTF8Converter = CharEncodingConverter<
        UTF32CharEncoding, UTF8CharEncoding>;

    using UTF32ToUTF16Converter = CharEncodingConverter<
        UTF32CharEncoding, UTF16CharEncoding>;

    template <typename TInputIt>
    using UTF32ToUTF8LazyConverter = CharEncodingLazyConverter<
        UTF32CharEncoding, UTF8CharEncoding, TInputIt>;

    template <typename TInputIt>
    using UTF32ToUTF16LazyConverter = CharEncodingLazyConverter<
        UTF32CharEncoding, UTF16CharEncoding, TInputIt>;
}