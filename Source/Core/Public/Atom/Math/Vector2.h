#pragma once
#include "Atom/Math/Core.h"

namespace Atom
{
    struct SVector2
    {
        float x;
        float y;
    };

    constexpr SVector2 operator + (SVector2 lhs, SVector2 rhs) noex
    {
        return { lhs.x + rhs.x, lhs.y + rhs.y };
    }

    constexpr SVector2 operator - (SVector2 lhs, SVector2 rhs) noex
    {
        return { lhs.x - rhs.x, lhs.y - rhs.y };
    }

    constexpr SVector2 operator * (SVector2 lhs, float scalar) noex
    {
        return { lhs.x * scalar, lhs.y * scalar };
    }

    constexpr SVector2 operator / (SVector2 lhs, float scalar) noex
    {
        return { lhs.x / scalar, lhs.y / scalar };
    }

    constexpr SVector2& operator += (SVector2& lhs, const SVector2& rhs) noex
    {
        lhs.x += rhs.x;
        lhs.y += rhs.y;
        return lhs;
    }

    constexpr SVector2& operator -= (SVector2& lhs, const SVector2& rhs) noex
    {
        lhs.x -= rhs.x;
        lhs.y -= rhs.y;
        return lhs;
    }

    constexpr SVector2& operator *= (SVector2& lhs, float scalar) noex
    {
        lhs.x *= scalar;
        lhs.y *= scalar;
        return lhs;
    }

    constexpr SVector2& operator /= (SVector2& lhs, float scalar) noex
    {
        lhs.x /= scalar;
        lhs.y /= scalar;
        return lhs;
    }

    constexpr bool operator == (SVector2 lhs, SVector2 rhs) noex
    {
        return Math::IsApproximatelyZero(lhs.x, rhs.x) &&
            Math::IsApproximatelyZero(lhs.y, rhs.y);
    }

    constexpr bool operator != (SVector2 lhs, SVector2 rhs) noex
    {
        return !(lhs == rhs);
    }

    constexpr bool operator > (SVector2 lhs, SVector2 rhs) noex
    {
        return (lhs.x > rhs.x) && (lhs.y > rhs.y);
    }

    constexpr bool operator < (SVector2 lhs, SVector2 rhs) noex
    {
        return (lhs.x < rhs.x) && (lhs.y < rhs.y);
    }

    constexpr bool operator >= (SVector2 lhs, SVector2 rhs) noex
    {
        return (lhs.x >= rhs.x) && (lhs.y >= rhs.y);
    }

    constexpr bool operator <= (SVector2 lhs, SVector2 rhs) noex
    {
        return (lhs.x <= rhs.x) && (lhs.y <= rhs.y);
    }
}