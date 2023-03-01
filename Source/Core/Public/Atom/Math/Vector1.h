#pragma once
#include "Atom/Math/Core.h"

namespace Atom
{
    struct Vector1
    {
        float x;
    };

    constexpr Vector1 operator + (Vector1 lhs, Vector1 rhs) noexcept
    {
        return { lhs.x + rhs.x };
    }

    constexpr Vector1 operator - (Vector1 lhs, Vector1 rhs) noexcept
    {
        return { lhs.x - rhs.x };
    }

    constexpr Vector1 operator * (Vector1 lhs, float scalar) noexcept
    {
        return { lhs.x * scalar };
    }

    constexpr Vector1 operator / (Vector1 lhs, float scalar) noexcept
    {
        return { lhs.x / scalar };
    }

    constexpr Vector1& operator += (Vector1& lhs, const Vector1& rhs) noexcept
    {
        lhs.x += rhs.x;
        return lhs;
    }

    constexpr Vector1& operator -= (Vector1& lhs, const Vector1& rhs) noexcept
    {
        lhs.x -= rhs.x;
        return lhs;
    }

    constexpr Vector1& operator *= (Vector1& lhs, float scalar) noexcept
    {
        lhs.x *= scalar;
        return lhs;
    }

    constexpr Vector1& operator /= (Vector1& lhs, float scalar) noexcept
    {
        lhs.x /= scalar;
        return lhs;
    }

    constexpr bool operator == (Vector1 lhs, Vector1 rhs) noexcept
    {
        return Math::IsApproximatelyZero(lhs.x, rhs.x);
    }

    constexpr bool operator != (Vector1 lhs, Vector1 rhs) noexcept
    {
        return !(lhs == rhs);
    }

    constexpr bool operator > (Vector1 lhs, Vector1 rhs) noexcept
    {
        return lhs.x > rhs.x;
    }

    constexpr bool operator < (Vector1 lhs, Vector1 rhs) noexcept
    {
        return lhs.x < rhs.x;
    }

    constexpr bool operator >= (Vector1 lhs, Vector1 rhs) noexcept
    {
        return lhs.x >= rhs.x;
    }

    constexpr bool operator <= (Vector1 lhs, Vector1 rhs) noexcept
    {
        return lhs.x <= rhs.x;
    }
}