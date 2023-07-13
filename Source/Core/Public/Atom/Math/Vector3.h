#pragma once
#include "Atom/Math/Core.h"

namespace Atom
{
    class SVector3
    {
        pub float x;
        pub float y;
        pub float z;
    };

    cexpr fn operator + (SVector3 lhs, SVector3 rhs) noex -> SVector3
    {
        return { lhs.x + rhs.x, lhs.y + rhs.y };
    }

    cexpr fn operator - (SVector3 lhs, SVector3 rhs) noex -> SVector3
    {
        return { lhs.x - rhs.x, lhs.y - rhs.y };
    }

    cexpr fn operator * (SVector3 lhs, float scalar) noex -> SVector3
    {
        return { lhs.x * scalar, lhs.y * scalar };
    }

    cexpr fn operator / (SVector3 lhs, float scalar) noex -> SVector3
    {
        return { lhs.x / scalar, lhs.y / scalar };
    }

    cexpr fn operator += (SVector3& lhs, const SVector3& rhs) noex -> SVector3&
    {
        lhs.x += rhs.x;
        lhs.y += rhs.y;
        lhs.z += rhs.z;
        return lhs;
    }

    cexpr fn operator -= (SVector3& lhs, const SVector3& rhs) noex -> SVector3&
    {
        lhs.x -= rhs.x;
        lhs.y -= rhs.y;
        lhs.z -= rhs.z;
        return lhs;
    }

    cexpr fn operator *= (SVector3& lhs, float scalar) noex -> SVector3&
    {
        lhs.x *= scalar;
        lhs.y *= scalar;
        lhs.z *= scalar;
        return lhs;
    }

    cexpr fn operator /= (SVector3& lhs, float scalar) noex -> SVector3&
    {
        lhs.x /= scalar;
        lhs.y /= scalar;
        lhs.z /= scalar;
        return lhs;
    }

    cexpr fn operator == (SVector3 lhs, SVector3 rhs) noex -> bool
    {
        return Math::IsApproximatelyZero(lhs.x, rhs.x) &&
            Math::IsApproximatelyZero(lhs.y, rhs.y) &&
            Math::IsApproximatelyZero(lhs.z, rhs.z);
    }

    cexpr fn operator != (SVector3 lhs, SVector3 rhs) noex -> bool
    {
        return !(lhs == rhs);
    }

    cexpr fn operator > (SVector3 lhs, SVector3 rhs) noex -> bool
    {
        return (lhs.x > rhs.x) && (lhs.y > rhs.y) && (lhs.z > rhs.z);
    }

    cexpr fn operator < (SVector3 lhs, SVector3 rhs) noex -> bool
    {
        return (lhs.x < rhs.x) && (lhs.y < rhs.y) && (lhs.z < rhs.z);
    }

    cexpr fn operator >= (SVector3 lhs, SVector3 rhs) noex -> bool
    {
        return (lhs.x >= rhs.x) && (lhs.y >= rhs.y) && (lhs.z >= rhs.z);
    }

    cexpr fn operator <= (SVector3 lhs, SVector3 rhs) noex -> bool
    {
        return (lhs.x <= rhs.x) && (lhs.y <= rhs.y) && (lhs.z <= rhs.z);
    }
}