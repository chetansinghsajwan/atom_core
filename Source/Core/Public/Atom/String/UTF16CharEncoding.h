#pragma once
#include "Atom/String/BasicChar.h"
#include "Atom/String/BasicString.h"
#include "Atom/String/BasicStringView.h"

#define UTF16(lit) u##lit

namespace Atom
{
    struct UTF16CharEncoding
    {
        using TChar = char16;
        using TUnit = char16;

        static constexpr TChar Null = UTF16('\0');
        static constexpr bool IsMultiUnitEncoding = true;

        static constexpr bool IsContinuationUnit(TUnit unit) noexcept
        {
            return (unit & 0b11000000) == 0b10000000;
        }

        static constexpr usize ParseStartingUnit(TUnit unit) noexcept
        {
            return 0;
        }

        static constexpr bool IsStartingUnit(TUnit unit) noexcept
        {
            return ParseStartingUnit(unit) != 0;
        }
    };

    using UTF16Char = BasicChar<UTF16CharEncoding>;
    using UTF16Unit = UTF16Char;
    using UTF16Point = uint32;

    template <typename TAllocator>
    using UTF16BasicString = BasicString<UTF16CharEncoding, TAllocator>;

    using UTF16String = BasicString<UTF16CharEncoding>;

    using UTF16StringView = BasicStringView<UTF16CharEncoding>;
}