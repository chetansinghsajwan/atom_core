#pragma once

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// ascii char encoding.
    /// --------------------------------------------------------------------------------------------
    class ascii_encoding
    {
    public:
        using char_type = uchar;
        using rune_type = uchar;

    public:
        static constexpr char_type null = '\0';
        static constexpr bool is_multi_char_encoding = false;
    };

    /// --------------------------------------------------------------------------------------------
    /// {char} for {ascii_encoding}.
    /// --------------------------------------------------------------------------------------------
    using ascii_char = typename ascii_encoding::char_type;

    /// --------------------------------------------------------------------------------------------
    /// {rune} for {ascii_encoding}.
    /// --------------------------------------------------------------------------------------------
    using ascii_rune = typename ascii_encoding::rune_type;
}
