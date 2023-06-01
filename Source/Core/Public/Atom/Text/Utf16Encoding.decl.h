#pragma once
#include "Atom/Core.h"

#define ATOM_TEXT_UTF16(lit) u##lit

namespace Atom
{
    struct Utf16Encoding
    {
        using TChar = char16;
        using TRune = char32;

        static constexpr TChar Null = ATOM_TEXT_UTF16('\0');
        static constexpr bool IsMultiCharEncoding = true;

        static constexpr bool IsContinuationChar(TChar ch) noexcept
        {
            return (ch & 0b11000000) == 0b10000000;
        }

        static constexpr usize ParseStartingChar(TChar ch) noexcept
        {
            return 0;
        }

        static constexpr bool IsStartingChar(TChar ch) noexcept
        {
            return ParseStartingChar(ch) != 0;
        }
    };

    using Utf16Char = typename Utf16Encoding::TChar;
    using Utf16Rune = typename Utf16Encoding::TRune;
}