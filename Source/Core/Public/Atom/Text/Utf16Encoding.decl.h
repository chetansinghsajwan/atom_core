#pragma once
#include "Atom/Core.h"

#define ATOM_TEXT_UTF16(lit) u##lit

namespace Atom
{
    class Utf16Encoding
    {
        pub using TChar = char16;
        pub using TRune = char32;

        pub static cexpr TChar Null = ATOM_TEXT_UTF16('\0');
        pub static cexpr bool IsMultiCharEncoding = true;

        pub static cexpr fn IsContinuationChar(TChar ch) noex -> bool
        {
            return (ch & 0b11000000) == 0b10000000;
        }

        pub static cexpr fn ParseStartingChar(TChar ch) noex -> usize
        {
            return 0;
        }

        pub static cexpr fn IsStartingChar(TChar ch) noex -> bool
        {
            return ParseStartingChar(ch) != 0;
        }
    };

    using Utf16Char = tname Utf16Encoding::TChar;
    using Utf16Rune = tname Utf16Encoding::TRune;
}