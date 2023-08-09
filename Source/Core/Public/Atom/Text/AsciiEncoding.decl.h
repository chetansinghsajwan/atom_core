#pragma once

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// Ascii char encoding.
    /// --------------------------------------------------------------------------------------------
    class AsciiEncoding
    {
        pub using TChar = char;
        pub using TRune = char;

        pub static cexpr TChar Null = '\0';
        pub static cexpr bool IsMultiCharEncoding = false;
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