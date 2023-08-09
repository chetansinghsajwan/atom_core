#pragma once
#include "Atom/Core.h"

namespace Atom
{
    class Utf16Encoding
    {
        pub using TChar = char16;
        pub using TRune = char32;

        pub static cexpr TChar Null = u'\0';
        pub static cexpr bool IsMultiCharEncoding = true;

        pub static cexpr fn IsContinuationChar(TChar ch) -> bool
        {
            return (ch & 0b11000000) == 0b10000000;
        }

        pub static cexpr fn ParseStartingChar(TChar ch) -> usize
        {
            return 0;
        }

        pub static cexpr fn IsStartingChar(TChar ch) -> bool
        {
            return ParseStartingChar(ch) != 0;
        }
    };

    using Utf16Char = typename Utf16Encoding::TChar;
    using Utf16Rune = typename Utf16Encoding::TRune;
}