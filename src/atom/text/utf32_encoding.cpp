export module atom.core:text.utf32_encoding;

namespace atom::text
{
    export class utf32_encoding
    {
    public:
        using char_type = char32_t;
        using rune_type = char32_t;

    public:
        static constexpr char_type null = u'\0';
        static constexpr bool is_multi_char_encoding = false;
    };
}
