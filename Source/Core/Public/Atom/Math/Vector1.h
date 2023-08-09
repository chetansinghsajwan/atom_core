#pragma once
#include "Atom/Math/Core.h"

namespace Atom
{
    class SVector1
    {
        pub f32 x;
    };

    cexpr fn op+(SVector1 lhs, SVector1 rhs) -> SVector1
    {
        return { lhs.x + rhs.x };
    }

    cexpr fn op-(SVector1 lhs, SVector1 rhs) -> SVector1
    {
        return { lhs.x - rhs.x };
    }

    cexpr fn op*(SVector1 lhs, f32 scalar) -> SVector1
    {
        return { lhs.x * scalar };
    }

    cexpr fn op/(SVector1 lhs, f32 scalar) -> SVector1
    {
        return { lhs.x / scalar };
    }

    cexpr fn op+=(SVector1& lhs, const SVector1& rhs) -> SVector1&
    {
        lhs.x += rhs.x;
        return lhs;
    }

    cexpr fn op-=(SVector1& lhs, const SVector1& rhs) -> SVector1&
    {
        lhs.x -= rhs.x;
        return lhs;
    }

    cexpr fn op*=(SVector1& lhs, f32 scalar) -> SVector1&
    {
        lhs.x *= scalar;
        return lhs;
    }

    cexpr fn op/=(SVector1& lhs, f32 scalar) -> SVector1&
    {
        lhs.x /= scalar;
        return lhs;
    }

    cexpr fn op==(SVector1 lhs, SVector1 rhs) -> bool
    {
        return Math::IsApproximatelyZero(lhs.x, rhs.x);
    }

    cexpr fn op!=(SVector1 lhs, SVector1 rhs) -> bool
    {
        return !(lhs == rhs);
    }

    cexpr fn op>(SVector1 lhs, SVector1 rhs) -> bool
    {
        return lhs.x > rhs.x;
    }

    cexpr fn op<(SVector1 lhs, SVector1 rhs) -> bool
    {
        return lhs.x < rhs.x;
    }

    cexpr fn op>=(SVector1 lhs, SVector1 rhs) -> bool
    {
        return lhs.x >= rhs.x;
    }

    cexpr fn op<=(SVector1 lhs, SVector1 rhs) -> bool
    {
        return lhs.x <= rhs.x;
    }
}