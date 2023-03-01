#pragma once
#include "Atom/Math/Core.h"

namespace Atom
{
    struct Vector2
    {
        float x;
        float y;
    };

    constexpr Vector2 operator + (Vector2 lhs, Vector2 rhs) noexcept
    {
        return { lhs.x + rhs.x, lhs.y + rhs.y };
    }

    constexpr Vector2 operator - (Vector2 lhs, Vector2 rhs) noexcept
    {
        return { lhs.x - rhs.x, lhs.y - rhs.y };
    }

    constexpr Vector2 operator * (Vector2 lhs, float scalar) noexcept
    {
        return { lhs.x * scalar, lhs.y * scalar };
    }

    constexpr Vector2 operator / (Vector2 lhs, float scalar) noexcept
    {
        return { lhs.x / scalar, lhs.y / scalar };
    }

    constexpr Vector2& operator += (Vector2& lhs, const Vector2& rhs) noexcept
    {
        lhs.x += rhs.x;
        lhs.y += rhs.y;
        return lhs;
    }

    constexpr Vector2& operator -= (Vector2& lhs, const Vector2& rhs) noexcept
    {
        lhs.x -= rhs.x;
        lhs.y -= rhs.y;
        return lhs;
    }

    constexpr Vector2& operator *= (Vector2& lhs, float scalar) noexcept
    {
        lhs.x *= scalar;
        lhs.y *= scalar;
        return lhs;
    }

    constexpr Vector2& operator /= (Vector2& lhs, float scalar) noexcept
    {
        lhs.x /= scalar;
        lhs.y /= scalar;
        return lhs;
    }

    constexpr bool operator == (Vector2 lhs, Vector2 rhs) noexcept
    {
        return Math::IsApproximatelyZero(lhs.x, rhs.x) &&
            Math::IsApproximatelyZero(lhs.y, rhs.y);
    }

    constexpr bool operator != (Vector2 lhs, Vector2 rhs) noexcept
    {
        return !(lhs == rhs);
    }

    constexpr bool operator > (Vector2 lhs, Vector2 rhs) noexcept
    {
        return (lhs.x > rhs.x) && (lhs.y > rhs.y);
    }

    constexpr bool operator < (Vector2 lhs, Vector2 rhs) noexcept
    {
        return (lhs.x < rhs.x) && (lhs.y < rhs.y);
    }

    constexpr bool operator >= (Vector2 lhs, Vector2 rhs) noexcept
    {
        return (lhs.x >= rhs.x) && (lhs.y >= rhs.y);
    }

    constexpr bool operator <= (Vector2 lhs, Vector2 rhs) noexcept
    {
        return (lhs.x <= rhs.x) && (lhs.y <= rhs.y);
    }
}