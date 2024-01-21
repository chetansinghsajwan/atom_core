#pragma once

namespace atom::text
{
    class utf16_encoding
    {
    public:
        using char_type = char16_t;
        using rune_type = char32_t;

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
}
