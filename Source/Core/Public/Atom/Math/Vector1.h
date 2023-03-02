#pragma once
#include "Atom/Math/Core.h"

namespace Atom
{
    struct SVector1
    {
        float x;
    };

    constexpr SVector1 operator + (SVector1 lhs, SVector1 rhs) noexcept
    {
        return { lhs.x + rhs.x };
    }

    constexpr SVector1 operator - (SVector1 lhs, SVector1 rhs) noexcept
    {
        return { lhs.x - rhs.x };
    }

    constexpr SVector1 operator * (SVector1 lhs, float scalar) noexcept
    {
        return { lhs.x * scalar };
    }

    constexpr SVector1 operator / (SVector1 lhs, float scalar) noexcept
    {
        return { lhs.x / scalar };
    }

    constexpr SVector1& operator += (SVector1& lhs, const SVector1& rhs) noexcept
    {
        lhs.x += rhs.x;
        return lhs;
    }

    constexpr SVector1& operator -= (SVector1& lhs, const SVector1& rhs) noexcept
    {
        lhs.x -= rhs.x;
        return lhs;
    }

    constexpr SVector1& operator *= (SVector1& lhs, float scalar) noexcept
    {
        lhs.x *= scalar;
        return lhs;
    }

    constexpr SVector1& operator /= (SVector1& lhs, float scalar) noexcept
    {
        lhs.x /= scalar;
        return lhs;
    }

    constexpr bool operator == (SVector1 lhs, SVector1 rhs) noexcept
    {
        return Math::IsApproximatelyZero(lhs.x, rhs.x);
    }

    constexpr bool operator != (SVector1 lhs, SVector1 rhs) noexcept
    {
        return !(lhs == rhs);
    }

    constexpr bool operator > (SVector1 lhs, SVector1 rhs) noexcept
    {
        return lhs.x > rhs.x;
    }

    constexpr bool operator < (SVector1 lhs, SVector1 rhs) noexcept
    {
        return lhs.x < rhs.x;
    }

    constexpr bool operator >= (SVector1 lhs, SVector1 rhs) noexcept
    {
        return lhs.x >= rhs.x;
    }

    constexpr bool operator <= (SVector1 lhs, SVector1 rhs) noexcept
    {
        return lhs.x <= rhs.x;
    }
}