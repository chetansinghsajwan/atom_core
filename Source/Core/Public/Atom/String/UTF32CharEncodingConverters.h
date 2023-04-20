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
        template <RInputIterator<UTF32Char> TInput, ROutputWriter<UTF8Char> TOutput>
        static constexpr void ConvertChar(TInput in, TOutput out)
        {
            static constexpr UTF8StringView msg = UTF8(
                "UTF32 -> UTF8 char encoding conversion not implemented yet.");

            out += msg;
        }
    };

    template < >
    struct CharEncodingConversionImpl<UTF32CharEncoding, UTF16CharEncoding>
    {
        template <RInputIterator<UTF32Char> TInput, ROutputWriter<UTF16Char> TOutput>
        static constexpr void ConvertChar(TInput in, TOutput out)
        {
            static constexpr UTF16StringView msg = UTF16(
                "UTF32 -> UTF16 char encoding conversion not implemented yet.");

            out += msg;
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