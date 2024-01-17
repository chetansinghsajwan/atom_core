#pragma once

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// ascii char encoding.
    /// --------------------------------------------------------------------------------------------
    class ascii_encoding
    {
    public:
        using tchar = uchar;
        using trune = uchar;

    public:
        static constexpr tchar null = '\0';
        static constexpr bool is_multi_char_encoding = false;
    };

    /// --------------------------------------------------------------------------------------------
    /// {char} for {ascii_encoding}.
    /// --------------------------------------------------------------------------------------------
    using ascii_char = typename ascii_encoding::tchar;

    /// --------------------------------------------------------------------------------------------
    /// {rune} for {ascii_encoding}.
    /// --------------------------------------------------------------------------------------------
    using ascii_rune = typename ascii_encoding::trune;
}
