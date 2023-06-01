#pragma once

#define ATOM_TXT_ASCII(lit) lit

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// Ascii char encoding.
    /// --------------------------------------------------------------------------------------------
    struct AsciiEncoding
    {
        using TChar = char;
        using TRune = char;

        static constexpr TChar Null = ATOM_TXT_ASCII('\0');
        static constexpr bool IsMultiCharEncoding = false;
    };

    /// --------------------------------------------------------------------------------------------
    /// {Char} for {AsciiEncoding}.
    /// --------------------------------------------------------------------------------------------
    using AsciiChar = typename AsciiEncoding::TChar;

    /// --------------------------------------------------------------------------------------------
    /// {Rune} for {AsciiEncoding}.
    /// --------------------------------------------------------------------------------------------
    using AsciiRune = typename AsciiEncoding::TRune;
}