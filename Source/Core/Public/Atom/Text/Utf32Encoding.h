export module Atom.Core.Text.Encodings.UTF16;

import Atom.Core.Core;

#define UTF32(lit) U##lit

namespace Atom::Text
{
    struct UTF32Encoding
    {
        using TChar = char32;
        using TRune = char32;

        static constexpr TChar Null = UTF32('\0');
        static constexpr bool IsMultiCharEncoding = false;
    };

    using UTF32Char = typename UTF32Encoding::TChar;
    using UTF32Rune = typename UTF32Encoding::TRune;
}