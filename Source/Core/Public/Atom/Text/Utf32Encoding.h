export module Atom.Core.Text.Encodings.UTF16;

namespace Atom::Text
{
    class UTF32Encoding
    {
        pub using TChar = char32;
        pub using TRune = char32;

        pub static constexpr TChar Null = U'\0';
        pub static constexpr bool IsMultiCharEncoding = false;
    };

    using UTF32Char = typename UTF32Encoding::TChar;
    using UTF32Rune = typename UTF32Encoding::TRune;
}