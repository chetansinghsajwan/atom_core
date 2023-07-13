export module Atom.Core.Text.Encodings.UTF16;

import Atom.Core.Core;

#define UTF32(lit) U##lit

namespace Atom::Text
{
    class UTF32Encoding
    {
        pub using TChar = char32;
        pub using TRune = char32;

        pub static cexpr TChar Null = UTF32('\0');
        pub static cexpr bool IsMultiCharEncoding = false;
    };

    using UTF32Char = tname UTF32Encoding::TChar;
    using UTF32Rune = tname UTF32Encoding::TRune;
}