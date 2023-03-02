#pragma once
#include "Atom/Math/Core.h"

namespace Atom
{
    struct SVector2
    {
        float x;
        float y;
    };

    constexpr SVector2 operator + (SVector2 lhs, SVector2 rhs) noexcept
    {
        return { lhs.x + rhs.x, lhs.y + rhs.y };
    }

    constexpr SVector2 operator - (SVector2 lhs, SVector2 rhs) noexcept
    {
        return { lhs.x - rhs.x, lhs.y - rhs.y };
    }

    constexpr SVector2 operator * (SVector2 lhs, float scalar) noexcept
    {
        return { lhs.x * scalar, lhs.y * scalar };
    }

    constexpr SVector2 operator / (SVector2 lhs, float scalar) noexcept
    {
        return { lhs.x / scalar, lhs.y / scalar };
    }

    constexpr SVector2& operator += (SVector2& lhs, const SVector2& rhs) noexcept
    {
        lhs.x += rhs.x;
        lhs.y += rhs.y;
        return lhs;
    }

    constexpr SVector2& operator -= (SVector2& lhs, const SVector2& rhs) noexcept
    {
        lhs.x -= rhs.x;
        lhs.y -= rhs.y;
        return lhs;
    }

    constexpr SVector2& operator *= (SVector2& lhs, float scalar) noexcept
    {
        lhs.x *= scalar;
        lhs.y *= scalar;
        return lhs;
    }

    constexpr SVector2& operator /= (SVector2& lhs, float scalar) noexcept
    {
        lhs.x /= scalar;
        lhs.y /= scalar;
        return lhs;
    }

    constexpr bool operator == (SVector2 lhs, SVector2 rhs) noexcept
    {
        return Math::IsApproximatelyZero(lhs.x, rhs.x) &&
            Math::IsApproximatelyZero(lhs.y, rhs.y);
    }

    constexpr bool operator != (SVector2 lhs, SVector2 rhs) noexcept
    {
        return !(lhs == rhs);
    }

    constexpr bool operator > (SVector2 lhs, SVector2 rhs) noexcept
    {
        return (lhs.x > rhs.x) && (lhs.y > rhs.y);
    }

    constexpr bool operator < (SVector2 lhs, SVector2 rhs) noexcept
    {
        return (lhs.x < rhs.x) && (lhs.y < rhs.y);
    }

    constexpr bool operator >= (SVector2 lhs, SVector2 rhs) noexcept
    {
        return (lhs.x >= rhs.x) && (lhs.y >= rhs.y);
    }

    constexpr bool operator <= (SVector2 lhs, SVector2 rhs) noexcept
    {
        return (lhs.x <= rhs.x) && (lhs.y <= rhs.y);
    }
}