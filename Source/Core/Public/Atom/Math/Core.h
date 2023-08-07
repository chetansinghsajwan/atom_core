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
    cexpr fn Abs(f64 value) noex -> f64
    {
        // TODO: Improve this, maybe using sign bit flag.
        return value * (value < 0 ? -1 : 1);
    }

    cexpr fn IsApproximatelyZero(f64 lhs, f64 rhs) noex -> bool
    {
        return Abs(lhs - rhs) < 0.0001f;
    }

    template <tname TInt>
    cexpr fn Clamp(const TInt& value, const TInt& lhs, const TInt& rhs) -> TInt
    {
        if (value < lhs) return lhs;
        if (value > rhs) return rhs;

        return value;
    }

    // TODO: Implement with template args
    template <tname TInt>
    cexpr fn Min(const TInt& lhs, const TInt& rhs) -> TInt
    {
        return lhs < rhs ? lhs : rhs;
    }

    // TODO: Implement with template args
    template <tname TInt>
    cexpr fn Max(const TInt& lhs, const TInt& rhs) -> TInt
    {
        return lhs > rhs ? lhs : rhs;
    }

    cexpr fn CharToHex(Char ch) noex -> byte
    {
        if (ch >= '0' && ch <= '9')
            return byte(ch - '0');
        if (ch >= 'a' && ch <= 'f')
            return byte(10 + ch - 'a');
        if (ch >= 'A' && ch <= 'F')
            return byte(10 + ch - 'A');

        return -1;
    }

    cexpr fn HexToChar(byte hex) noex -> StaticStr<2>
    {
        cexpr const Char chars[] = "0123456789abcdef";

        byte high = hex >> 4;
        byte low = hex & 0b00001111;
        return { chars[high], chars[low] };
    }

    cexpr fn IsHexChar(Char ch) noex -> bool
    {
        return (ch >= '0' && ch <= '9') ||
            (ch >= 'a' && ch <= 'f') ||
            (ch >= 'A' && ch <= 'F');
    }
}