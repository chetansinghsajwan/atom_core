#pragma once
#include "Atom/Core.h"

namespace Atom
{
    class Utf16Encoding
    {
    public:
        using TChar = char16;
        using TRune = char32;

    public:
        static constexpr TChar Null = u'\0';
        static constexpr bool IsMultiCharEncoding = true;

    public:
        static constexpr auto IsContinuationChar(TChar ch) -> bool
        {
            return (ch & 0b11000000) == 0b10000000;
        }

        static constexpr auto ParseStartingChar(TChar ch) -> usize
        {
            return 0;
        }

        static constexpr auto IsStartingChar(TChar ch) -> bool
        {
            return ParseStartingChar(ch) != 0;
        }
    };

    using Utf16Char = typename Utf16Encoding::TChar;
    using Utf16Rune = typename Utf16Encoding::TRune;
}