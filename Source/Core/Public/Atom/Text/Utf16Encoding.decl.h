#pragma once
#include "Atom/Core.h"

#define ATOM_TEXT_UTF16(lit) u##lit

namespace Atom
{
    struct Utf16Encoding
    {
        using TChar = char16;
        using TRune = char32;

        static cexpr TChar Null = ATOM_TEXT_UTF16('\0');
        static cexpr bool IsMultiCharEncoding = true;

        static cexpr bool IsContinuationChar(TChar ch) noex
        {
            return (ch & 0b11000000) == 0b10000000;
        }

        static cexpr usize ParseStartingChar(TChar ch) noex
        {
            return 0;
        }

        static cexpr bool IsStartingChar(TChar ch) noex
        {
            return ParseStartingChar(ch) != 0;
        }
    };

    using Utf16Char = tname Utf16Encoding::TChar;
    using Utf16Rune = tname Utf16Encoding::TRune;
}