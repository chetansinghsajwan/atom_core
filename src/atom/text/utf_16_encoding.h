#pragma once
#include "atom/core.h"

namespace atom
{
    class utf16encoding
    {
    public:
        using tchar = char16;
        using trune = char32;

    public:
        static constexpr tchar null = u'\0';
        static constexpr bool is_multi_char_encoding = true;

    public:
        static constexpr auto is_continuation_char(tchar ch) -> bool
        {
            return (ch & 0b11000000) == 0b10000000;
        }

        static constexpr auto parse_starting_char(tchar ch) -> usize
        {
            return 0;
        }

        static constexpr auto is_starting_char(tchar ch) -> bool
        {
            return parse_starting_char(ch) != 0;
        }
    };

    using utf16char = typename utf16encoding::tchar;
    using utf16rune = typename utf16encoding::trune;
}
