#pragma once
#include "Atom/String/BasicChar.h"
#include "Atom/String/BasicString.h"
#include "Atom/String/BasicStringView.h"

#define UTF8(lit) u8##lit

namespace Atom
{
    struct UTF8CharEncoding
    {
        using TChar = char8_t;
        using TUnit = char8_t;

        static constexpr TChar Null = UTF8('\0');
        static constexpr bool IsMultiUnitEncoding = true;

        static constexpr bool IsContinuationUnit(TUnit unit) noexcept
        {
            return (unit & 0b11000000) == 0b10000000;
        }

        static constexpr usize ParseStartingUnit(TUnit unit) noexcept
        {
            if ((unit & 0b10000000) == 0b00000000) return 1;
            if ((unit & 0b11100000) == 0b11000000) return 2;
            if ((unit & 0b11110000) == 0b11100000) return 3;
            if ((unit & 0b11111000) == 0b11110000) return 4;

            return 0;
        }

        static constexpr bool IsStartingUnit(TUnit unit) noexcept
        {
            return ParseStartingUnit(unit) != 0;
        }
    };

    using UTF8Char = BasicChar<UTF8CharEncoding>;
    using UTF8Unit = UTF8Char;
    using UTF8Point = uint32;

    template <typename TAllocator>
    using UTF8BasicString = BasicString<UTF8CharEncoding, TAllocator>;

    using UTF8String = BasicString<UTF8CharEncoding>;

    using UTF8StringView = BasicStringView<UTF8CharEncoding>;
}