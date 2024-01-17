export module atom.core.text.encodings.utf16;

namespace atom::text
{
    class utf32encoding
    {
    public:
        using tchar = char32;
        using trune = char32;

    public:
        static constexpr tchar null = u'\0';
        static constexpr bool is_multi_char_encoding = false;
    };

    using utf32char = typename utf32encoding::tchar;
    using utf32rune = typename utf32encoding::trune;
}
