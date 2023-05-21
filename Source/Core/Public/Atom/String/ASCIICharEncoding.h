#pragma once
#include "Atom/String/BasicChar.h"
#include "Atom/String/BasicString.h"
#include "Atom/String/BasicStringView.h"

#define ASCII(lit) lit

namespace Atom
{
    struct ASCIICharEncoding
    {
        using TChar = char;

        static constexpr TChar Null = ASCII('\0');
        static constexpr bool IsMultiUnitEncoding = false;
    };

    using ASCIIChar = BasicChar<ASCIICharEncoding>;
    using ASCIIUnit = ASCIIChar;
    using ASCIIPoint = uint32;

    template <typename TAllocator>
    using ASCIIBasicString = BasicString<ASCIICharEncoding, TAllocator>;

    using ASCIIString = BasicString<ASCIICharEncoding>;

    using ASCIIStringView = BasicStringView<ASCIICharEncoding>;
}