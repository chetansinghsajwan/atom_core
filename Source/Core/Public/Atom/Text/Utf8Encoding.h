#pragma once
#include "Atom/Core/Core.h"

namespace Atom::Text
{
    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    export class Utf8Encoding
    {
    public:
        using TChar = char8;
        using TRune = char32;

    public:
        static constexpr TChar Null = u8('\0');
        static constexpr bool IsMultiCharEncoding = true;

    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        static constexpr fn IsContinuationChar(TChar ch) -> bool
        {
            return (ch & 0b11000000) == 0b10000000;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        static constexpr fn ParseStartingChar(TChar ch) -> usize
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
        static constexpr fn IsStartingChar(TChar ch) -> bool
        {
            return ParseStartingChar(ch) != 0;
        }
    };

    using UTF8Char = typename UTF8Encoding::TChar;
    using UTF8Rune = typename UTF8Encoding::TRune;
}