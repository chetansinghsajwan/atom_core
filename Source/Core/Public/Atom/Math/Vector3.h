#pragma once
#include "Atom/Math/Core.h"

namespace Atom
{
    class SVector3
    {
        pub f32 x;
        pub f32 y;
        pub f32 z;
    };

    cexpr fn op+(SVector3 lhs, SVector3 rhs) noex -> SVector3
    {
        return { lhs.x + rhs.x, lhs.y + rhs.y };
    }

    cexpr fn op-(SVector3 lhs, SVector3 rhs) noex -> SVector3
    {
        return { lhs.x - rhs.x, lhs.y - rhs.y };
    }

    cexpr fn op*(SVector3 lhs, f32 scalar) noex -> SVector3
    {
        return { lhs.x * scalar, lhs.y * scalar };
    }

    cexpr fn op/(SVector3 lhs, f32 scalar) noex -> SVector3
    {
        return { lhs.x / scalar, lhs.y / scalar };
    }

    cexpr fn op+=(SVector3& lhs, const SVector3& rhs) noex -> SVector3&
    {
        lhs.x += rhs.x;
        lhs.y += rhs.y;
        lhs.z += rhs.z;
        return lhs;
    }

    cexpr fn op-=(SVector3& lhs, const SVector3& rhs) noex -> SVector3&
    {
        lhs.x -= rhs.x;
        lhs.y -= rhs.y;
        lhs.z -= rhs.z;
        return lhs;
    }

    cexpr fn op*=(SVector3& lhs, f32 scalar) noex -> SVector3&
    {
        lhs.x *= scalar;
        lhs.y *= scalar;
        lhs.z *= scalar;
        return lhs;
    }

    cexpr fn op/=(SVector3& lhs, f32 scalar) noex -> SVector3&
    {
        lhs.x /= scalar;
        lhs.y /= scalar;
        lhs.z /= scalar;
        return lhs;
    }

    cexpr fn op==(SVector3 lhs, SVector3 rhs) noex -> bool
    {
        return Math::IsApproximatelyZero(lhs.x, rhs.x) &&
            Math::IsApproximatelyZero(lhs.y, rhs.y) &&
            Math::IsApproximatelyZero(lhs.z, rhs.z);
    }

    cexpr fn op!=(SVector3 lhs, SVector3 rhs) noex -> bool
    {
        return !(lhs == rhs);
    }

    cexpr fn op>(SVector3 lhs, SVector3 rhs) noex -> bool
    {
        return (lhs.x > rhs.x) && (lhs.y > rhs.y) && (lhs.z > rhs.z);
    }

    cexpr fn op<(SVector3 lhs, SVector3 rhs) noex -> bool
    {
        return (lhs.x < rhs.x) && (lhs.y < rhs.y) && (lhs.z < rhs.z);
    }

    cexpr fn op>=(SVector3 lhs, SVector3 rhs) noex -> bool
    {
        return (lhs.x >= rhs.x) && (lhs.y >= rhs.y) && (lhs.z >= rhs.z);
    }

    cexpr fn op<=(SVector3 lhs, SVector3 rhs) noex -> bool
    {
        return (lhs.x <= rhs.x) && (lhs.y <= rhs.y) && (lhs.z <= rhs.z);
    }
}