#pragma once
#include "Atom/String/BasicString.h"
#include "Atom/String/BasicStringView.h"

#define UTF32(lit) U##lit

namespace Atom
{
    struct UTF32CharEncoding
    {
        using TChar = char32_t;

        static constexpr TChar Null = UTF32('\0');
        static constexpr bool IsMultiUnitEncoding = false;
    };

    using UTF32Char = BasicChar<UTF32CharEncoding>;
    using UTF32String = BasicString<UTF32CharEncoding>;
    using UTF32StringView = BasicStringView<UTF32CharEncoding>;

    template <typename TAllocator>
    using UTF32BasicString = BasicString<UTF32CharEncoding, TAllocator>;

    using UTF32String = BasicString<UTF32CharEncoding>;

    using UTF32StringView = BasicStringView<UTF32CharEncoding>;
}