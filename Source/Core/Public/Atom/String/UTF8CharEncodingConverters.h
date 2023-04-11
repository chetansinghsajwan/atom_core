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
        template <RConstIterator<UTF8Char> TIn, RBackInsertable<UTF16Char> TOut>
        static constexpr void ConvertChar(TIn& in, TIn& in_end, TOut& out)
        {
            UTF16StringView msg = TEXT("UTF8 -> UTF16 char encoding conversion not"
                " implemented yet.");

            out.InsertBack(BasicStringViewWrapper<UTF16CharEncoding>{ msg });
        }
    };

    template < >
    struct CharEncodingConversionImpl<UTF8CharEncoding, UTF32CharEncoding>
    {
        template <RConstIterator<UTF8Char> TIn, RBackInsertable<UTF32Char> TOut>
        static constexpr void ConvertChar(TIn& in, TIn& in_end, TOut& out)
        {
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