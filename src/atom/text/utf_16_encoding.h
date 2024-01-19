#pragma once
#include "atom/core.h"

namespace atom
{
    class utf16encoding
    {
    public:
        using char_type = char16;
        using rune_type = char32;

    public:
        static constexpr char_type null = u'\0';
        static constexpr bool is_multi_char_encoding = true;

    public:
        static constexpr auto is_continuation_char(char_type ch) -> bool
        {
            return (ch & 0b11000000) == 0b10000000;
        }

        static constexpr auto parse_starting_char(char_type ch) -> usize
        {
            return 0;
        }

        static constexpr auto is_starting_char(char_type ch) -> bool
        {
            return parse_starting_char(ch) != 0;
        }
    };

    using utf16char = typename utf16encoding::char_type;
    using utf16rune = typename utf16encoding::rune_type;
}
