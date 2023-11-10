#pragma once

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// Ascii char encoding.
    /// --------------------------------------------------------------------------------------------
    class AsciiEncoding
    {
    public:
        using TChar = Char;
        using TRune = Char;

    public:
        static constexpr TChar Null = '\0';
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
