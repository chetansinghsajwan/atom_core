#pragma once
#include <numeric>

#include "Atom/Core.h"
#include "Atom/Str/StaticStr.h"

namespace Atom
{
    template <tname T>
    using NumLimits = std::numeric_limits<T>;
}

namespace Atom::Math
{
    constexpr fn Abs(double value) noex -> double
    {
        // TODO: Improve this, maybe using sign bit flag.
        return value * (value < 0 ? -1 : 1);
    }

    constexpr fn IsApproximatelyZero(double lhs, double rhs) noex -> bool
    {
        return Abs(lhs - rhs) < 0.0001f;
    }

    template <tname TInt>
    constexpr fn Clamp(const TInt& value, const TInt& lhs, const TInt& rhs) -> TInt
    {
        if (value < lhs) return lhs;
        if (value > rhs) return rhs;

        return value;
    }

    // TODO: Implement with template args
    template <tname TInt>
    constexpr fn Min(const TInt& lhs, const TInt& rhs) -> TInt
    {
        return lhs < rhs ? lhs : rhs;
    }

    // TODO: Implement with template args
    template <tname TInt>
    constexpr fn Max(const TInt& lhs, const TInt& rhs) -> TInt
    {
        return lhs > rhs ? lhs : rhs;
    }

    constexpr fn CharToHex(Char ch) noex -> byte
    {
        if (ch >= TEXT('0') && ch <= TEXT('9'))
            return byte(ch - TEXT('0'));
        if (ch >= TEXT('a') && ch <= TEXT('f'))
            return byte(10 + ch - TEXT('a'));
        if (ch >= TEXT('A') && ch <= TEXT('F'))
            return byte(10 + ch - TEXT('A'));

        return -1;
    }

    constexpr fn HexToChar(byte hex) noex -> StaticStr<2>
    {
        constexpr const Char chars[] = TEXT("0123456789abcdef");

        byte high = hex >> 4;
        byte low = hex & 0b00001111;
        return { chars[high], chars[low] };
    }

    constexpr fn IsHexChar(Char ch) noex -> bool
    {
        return (ch >= TEXT('0') && ch <= TEXT('9')) ||
            (ch >= TEXT('a') && ch <= TEXT('f')) ||
            (ch >= TEXT('A') && ch <= TEXT('F'));
    }
}