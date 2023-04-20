#pragma once
#include "Atom/String/CharEncodingConverter.h"
#include "Atom/String/UTF8CharEncoding.h"
#include "Atom/String/UTF16CharEncoding.h"
#include "Atom/String/UTF32CharEncoding.h"

namespace Atom::Private
{
    template < >
    struct CharEncodingConversionImpl<UTF8CharEncoding, UTF16CharEncoding>
    {
        template <RInputIterator<UTF8Char> TInput, ROutputWriter<UTF16Char> TOutput>
        static constexpr void ConvertChar(TInput in, TOutput out)
        {
            static constexpr UTF16StringView msg = UTF16(
                "UTF8 -> UTF16 char encoding conversion not implemented yet.");

            out += msg;
        }
    };

    template < >
    struct CharEncodingConversionImpl<UTF8CharEncoding, UTF32CharEncoding>
    {
        template <RInputIterator<UTF8Char> TInput, ROutputWriter<UTF32Char> TOutput>
        static constexpr void ConvertChar(TInput in, TOutput out)
        {
            static constexpr UTF32StringView msg = UTF32(
                "UTF8 -> UTF32 char encoding conversion not implemented yet.");

            out += msg;
        }
    };
}

namespace Atom
{
    static_assert(RCharEncodingConvertible<UTF8CharEncoding, UTF8CharEncoding>,
        "No CharEncodingConverter<UTF8CharEncoding, UTF8CharEncoding> specialization specified.");

    static_assert(RCharEncodingConvertible<UTF8CharEncoding, UTF16CharEncoding>,
        "No CharEncodingConverter<UTF8CharEncoding, UTF16CharEncoding> specialization specified.");

    static_assert(RCharEncodingConvertible<UTF8CharEncoding, UTF32CharEncoding>,
        "No CharEncodingConverter<UTF8CharEncoding, UTF32CharEncoding> specialization specified.");

    static_assert(RCharEncodingLazyConvertible<UTF8CharEncoding, UTF8CharEncoding>,
        "No CharEncodingLazyConverter<UTF8CharEncoding, UTF8CharEncoding> specialization specified.");

    static_assert(RCharEncodingLazyConvertible<UTF8CharEncoding, UTF16CharEncoding>,
        "No CharEncodingLazyConverter<UTF8CharEncoding, UTF16CharEncoding> specialization specified.");

    static_assert(RCharEncodingLazyConvertible<UTF8CharEncoding, UTF32CharEncoding>,
        "No CharEncodingLazyConverter<UTF8CharEncoding, UTF32CharEncoding> specialization specified.");

    using UTF8ToUTF16Converter = CharEncodingConverter<
        UTF8CharEncoding, UTF16CharEncoding>;

    using UTF8ToUTF32Converter = CharEncodingConverter<
        UTF8CharEncoding, UTF32CharEncoding>;

    template <typename TInputIt>
    using UTF8ToUTF16LazyConverter = CharEncodingLazyConverter<
        UTF8CharEncoding, UTF16CharEncoding, TInputIt>;

    template <typename TInputIt>
    using UTF8ToUTF32LazyConverter = CharEncodingLazyConverter<
        UTF8CharEncoding, UTF32CharEncoding, TInputIt>;
}