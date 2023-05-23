#pragma once
#include "Atom/String/BasicChar.h"

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
}