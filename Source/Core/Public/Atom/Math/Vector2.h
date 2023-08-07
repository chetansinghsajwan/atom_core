#pragma once
#include "Atom/Math/Core.h"

namespace Atom
{
    class SVector2
    {
        pub f32 x;
        pub f32 y;
    };

    cexpr fn op+(SVector2 lhs, SVector2 rhs) noex -> SVector2
    {
        return { lhs.x + rhs.x, lhs.y + rhs.y };
    }

    cexpr fn op-(SVector2 lhs, SVector2 rhs) noex -> SVector2
    {
        return { lhs.x - rhs.x, lhs.y - rhs.y };
    }

    cexpr fn op*(SVector2 lhs, f32 scalar) noex -> SVector2
    {
        return { lhs.x * scalar, lhs.y * scalar };
    }

    cexpr fn op/(SVector2 lhs, f32 scalar) noex -> SVector2
    {
        return { lhs.x / scalar, lhs.y / scalar };
    }

    cexpr fn op+=(SVector2& lhs, const SVector2& rhs) noex -> SVector2&
    {
        lhs.x += rhs.x;
        lhs.y += rhs.y;
        return lhs;
    }

    cexpr fn op-=(SVector2& lhs, const SVector2& rhs) noex -> SVector2&
    {
        lhs.x -= rhs.x;
        lhs.y -= rhs.y;
        return lhs;
    }

    cexpr fn op*=(SVector2& lhs, f32 scalar) noex -> SVector2&
    {
        lhs.x *= scalar;
        lhs.y *= scalar;
        return lhs;
    }

    cexpr fn op/=(SVector2& lhs, f32 scalar) noex -> SVector2&
    {
        lhs.x /= scalar;
        lhs.y /= scalar;
        return lhs;
    }

    cexpr fn op==(SVector2 lhs, SVector2 rhs) noex -> bool
    {
        return Math::IsApproximatelyZero(lhs.x, rhs.x) &&
            Math::IsApproximatelyZero(lhs.y, rhs.y);
    }

    cexpr fn op!=(SVector2 lhs, SVector2 rhs) noex -> bool
    {
        return !(lhs == rhs);
    }

    cexpr fn op>(SVector2 lhs, SVector2 rhs) noex -> bool
    {
        return (lhs.x > rhs.x) && (lhs.y > rhs.y);
    }

    cexpr fn op<(SVector2 lhs, SVector2 rhs) noex -> bool
    {
        return (lhs.x < rhs.x) && (lhs.y < rhs.y);
    }

    cexpr fn op>=(SVector2 lhs, SVector2 rhs) noex -> bool
    {
        return (lhs.x >= rhs.x) && (lhs.y >= rhs.y);
    }

    cexpr fn op<=(SVector2 lhs, SVector2 rhs) noex -> bool
    {
        return (lhs.x <= rhs.x) && (lhs.y <= rhs.y);
    }
}