#pragma once
#include <numeric>

#include "Atom/Core.h"
#include "Atom/Str/StaticStr.h"

namespace Atom
{
    template <typename T>
    using NumLimits = std::numeric_limits<T>;
}

namespace Atom::Math
{
    constexpr fn Abs(f64 value) -> f64
    {
        // TODO: Improve this, maybe using sign bit flag.
        return value * (value < 0 ? -1 : 1);
    }

    constexpr fn IsApproximatelyZero(f64 lhs, f64 rhs) -> bool
    {
        return Abs(lhs - rhs) < 0.0001f;
    }

    template <typename TInt>
    constexpr fn Clamp(const TInt& value, const TInt& lhs, const TInt& rhs) -> TInt
    {
        if (value < lhs) return lhs;
        if (value > rhs) return rhs;

        return value;
    }

    // TODO: Implement with template args
    template <typename TInt>
    constexpr fn Min(const TInt& lhs, const TInt& rhs) -> TInt
    {
        return lhs < rhs ? lhs : rhs;
    }

    // TODO: Implement with template args
    template <typename TInt>
    constexpr fn Max(const TInt& lhs, const TInt& rhs) -> TInt
    {
        return lhs > rhs ? lhs : rhs;
    }

    constexpr fn CharToHex(Char ch) -> byte
    {
        if (ch >= '0' && ch <= '9')
            return byte(ch - '0');
        if (ch >= 'a' && ch <= 'f')
            return byte(10 + ch - 'a');
        if (ch >= 'A' && ch <= 'F')
            return byte(10 + ch - 'A');

        return -1;
    }

    constexpr fn HexToChar(byte hex) -> StaticStr<2>
    {
        constexpr const Char chars[] = "0123456789abcdef";

        byte high = hex >> 4;
        byte low = hex & 0b00001111;
        return { chars[high], chars[low] };
    }

    constexpr fn IsHexChar(Char ch) -> bool
    {
        return (ch >= '0' && ch <= '9') ||
            (ch >= 'a' && ch <= 'f') ||
            (ch >= 'A' && ch <= 'F');
    }
}