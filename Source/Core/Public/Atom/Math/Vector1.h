#pragma once
#include "Atom/Math/Core.h"

namespace Atom
{
    class SVector1
    {
        pub float x;
    };

    cexpr fn operator + (SVector1 lhs, SVector1 rhs) noex -> SVector1
    {
        return { lhs.x + rhs.x };
    }

    cexpr fn operator - (SVector1 lhs, SVector1 rhs) noex -> SVector1
    {
        return { lhs.x - rhs.x };
    }

    cexpr fn operator * (SVector1 lhs, float scalar) noex -> SVector1
    {
        return { lhs.x * scalar };
    }

    cexpr fn operator / (SVector1 lhs, float scalar) noex -> SVector1
    {
        return { lhs.x / scalar };
    }

    cexpr fn operator += (SVector1& lhs, const SVector1& rhs) noex -> SVector1&
    {
        lhs.x += rhs.x;
        return lhs;
    }

    cexpr fn operator -= (SVector1& lhs, const SVector1& rhs) noex -> SVector1&
    {
        lhs.x -= rhs.x;
        return lhs;
    }

    cexpr fn operator *= (SVector1& lhs, float scalar) noex -> SVector1&
    {
        lhs.x *= scalar;
        return lhs;
    }

    cexpr fn operator /= (SVector1& lhs, float scalar) noex -> SVector1&
    {
        lhs.x /= scalar;
        return lhs;
    }

    cexpr fn operator == (SVector1 lhs, SVector1 rhs) noex -> bool
    {
        return Math::IsApproximatelyZero(lhs.x, rhs.x);
    }

    cexpr fn operator != (SVector1 lhs, SVector1 rhs) noex -> bool
    {
        return !(lhs == rhs);
    }

    cexpr fn operator > (SVector1 lhs, SVector1 rhs) noex -> bool
    {
        return lhs.x > rhs.x;
    }

    cexpr fn operator < (SVector1 lhs, SVector1 rhs) noex -> bool
    {
        return lhs.x < rhs.x;
    }

    cexpr fn operator >= (SVector1 lhs, SVector1 rhs) noex -> bool
    {
        return lhs.x >= rhs.x;
    }

    cexpr fn operator <= (SVector1 lhs, SVector1 rhs) noex -> bool
    {
        return lhs.x <= rhs.x;
    }
}