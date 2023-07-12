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

        static cexpr TChar Null = ATOM_TXT_ASCII('\0');
        static cexpr bool IsMultiCharEncoding = false;
    };

    /// --------------------------------------------------------------------------------------------
    /// {Char} for {AsciiEncoding}.
    /// --------------------------------------------------------------------------------------------
    using AsciiChar = tname AsciiEncoding::TChar;

    /// --------------------------------------------------------------------------------------------
    /// {Rune} for {AsciiEncoding}.
    /// --------------------------------------------------------------------------------------------
    using AsciiRune = tname AsciiEncoding::TRune;
}