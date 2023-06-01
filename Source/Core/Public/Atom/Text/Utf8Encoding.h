#pragma once
#include "Atom/Core/Core.h"

#define ATOM_TEXT_UTF8(lit) u8##lit

namespace Atom::Text
{
    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    export struct Utf8Encoding
    {
        using TChar = char8;
        using TRune = char32;

        static constexpr TChar Null = ATOM_TEXT_UTF8('\0');
        static constexpr bool IsMultiCharEncoding = true;

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        static constexpr bool IsContinuationChar(TChar ch) noexcept
        {
            return (ch & 0b11000000) == 0b10000000;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        static constexpr usize ParseStartingChar(TChar ch) noexcept
        {
            if ((ch & 0b10000000) == 0b00000000) return 1;
            if ((ch & 0b11100000) == 0b11000000) return 2;
            if ((ch & 0b11110000) == 0b11100000) return 3;
            if ((ch & 0b11111000) == 0b11110000) return 4;

            return 0;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        static constexpr bool IsStartingChar(TChar ch) noexcept
        {
            return ParseStartingChar(ch) != 0;
        }
    };

    using UTF8Char = typename UTF8Encoding::TChar;
    using UTF8Rune = typename UTF8Encoding::TRune;
}