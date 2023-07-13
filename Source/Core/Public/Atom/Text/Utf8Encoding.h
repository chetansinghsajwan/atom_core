#pragma once
#include "Atom/Core/Core.h"

#define ATOM_TEXT_UTF8(lit) u8##lit

namespace Atom::Text
{
    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    export class Utf8Encoding
    {
        pub using TChar = char8;
        pub using TRune = char32;

        pub static cexpr TChar Null = ATOM_TEXT_UTF8('\0');
        pub static cexpr bool IsMultiCharEncoding = true;

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        pub static cexpr fn IsContinuationChar(TChar ch) noex -> bool
        {
            return (ch & 0b11000000) == 0b10000000;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        pub static cexpr fn ParseStartingChar(TChar ch) noex -> usize
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
        pub static cexpr fn IsStartingChar(TChar ch) noex -> bool
        {
            return ParseStartingChar(ch) != 0;
        }
    };

    using UTF8Char = tname UTF8Encoding::TChar;
    using UTF8Rune = tname UTF8Encoding::TRune;
}