export module Atom.Core.Text.Encodings.UTF16;

namespace Atom::Text
{
    class UTF32Encoding
    {
    public:
        using TChar = char32;
        using TRune = char32;

    public:
        static constexpr TChar Null = U'\0';
        static constexpr bool IsMultiCharEncoding = false;
    };

    using UTF32Char = typename UTF32Encoding::TChar;
    using UTF32Rune = typename UTF32Encoding::TRune;
}
