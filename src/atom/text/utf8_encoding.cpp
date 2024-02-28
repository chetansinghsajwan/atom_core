export module atom.core:text.utf8_encoding;
import :core;

namespace atom::text
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    export class utf8_encoding
    {
    public:
        using char_type = char;
        using rune_type = char32_t;

    public:
        static constexpr char_type null = char_type('\0');
        static constexpr bool is_multi_char_encoding = true;

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static constexpr auto is_continuation_char(char_type ch) -> bool
        {
            return (ch & 0b11000000) == 0b10000000;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static constexpr auto parse_starting_char(char_type ch) -> usize
        {
            if ((ch & 0b10000000) == 0b00000000)
                return 1;
            if ((ch & 0b11100000) == 0b11000000)
                return 2;
            if ((ch & 0b11110000) == 0b11100000)
                return 3;
            if ((ch & 0b11111000) == 0b11110000)
                return 4;

            return 0;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        static constexpr auto is_starting_char(char_type ch) -> bool
        {
            return parse_starting_char(ch) != 0;
        }
    };
}
