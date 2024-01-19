export module atom.core.text.encodings.utf16;

namespace atom::text
{
    class utf32encoding
    {
    public:
        using char_type = char32;
        using rune_type = char32;

    public:
        static constexpr char_type null = u'\0';
        static constexpr bool is_multi_char_encoding = false;
    };

    using utf32char = typename utf32encoding::char_type;
    using utf32rune = typename utf32encoding::rune_type;
}
