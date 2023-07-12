#pragma once
#include "Atom/Math/Core.h"

namespace Atom
{
    struct SVector1
    {
        float x;
    };

    constexpr fn operator + (SVector1 lhs, SVector1 rhs) noex -> SVector1
    {
        return { lhs.x + rhs.x };
    }

    constexpr fn operator - (SVector1 lhs, SVector1 rhs) noex -> SVector1
    {
        return { lhs.x - rhs.x };
    }

    constexpr fn operator * (SVector1 lhs, float scalar) noex -> SVector1
    {
        return { lhs.x * scalar };
    }

    constexpr fn operator / (SVector1 lhs, float scalar) noex -> SVector1
    {
        return { lhs.x / scalar };
    }

    constexpr fn operator += (SVector1& lhs, const SVector1& rhs) noex -> SVector1&
    {
        lhs.x += rhs.x;
        return lhs;
    }

    constexpr fn operator -= (SVector1& lhs, const SVector1& rhs) noex -> SVector1&
    {
        lhs.x -= rhs.x;
        return lhs;
    }

    constexpr fn operator *= (SVector1& lhs, float scalar) noex -> SVector1&
    {
        lhs.x *= scalar;
        return lhs;
    }

    constexpr fn operator /= (SVector1& lhs, float scalar) noex -> SVector1&
    {
        lhs.x /= scalar;
        return lhs;
    }

    constexpr fn operator == (SVector1 lhs, SVector1 rhs) noex -> bool
    {
        return Math::IsApproximatelyZero(lhs.x, rhs.x);
    }

    constexpr fn operator != (SVector1 lhs, SVector1 rhs) noex -> bool
    {
        return !(lhs == rhs);
    }

    constexpr fn operator > (SVector1 lhs, SVector1 rhs) noex -> bool
    {
        return lhs.x > rhs.x;
    }

    constexpr fn operator < (SVector1 lhs, SVector1 rhs) noex -> bool
    {
        return lhs.x < rhs.x;
    }

    constexpr fn operator >= (SVector1 lhs, SVector1 rhs) noex -> bool
    {
        return lhs.x >= rhs.x;
    }

    constexpr fn operator <= (SVector1 lhs, SVector1 rhs) noex -> bool
    {
        return lhs.x <= rhs.x;
    }
}