#pragma once
#include "Atom/Math/Core.h"

namespace Atom
{
    struct SVector3
    {
        float x;
        float y;
        float z;
    };

    constexpr fn operator + (SVector3 lhs, SVector3 rhs) noex -> SVector3
    {
        return { lhs.x + rhs.x, lhs.y + rhs.y };
    }

    constexpr fn operator - (SVector3 lhs, SVector3 rhs) noex -> SVector3
    {
        return { lhs.x - rhs.x, lhs.y - rhs.y };
    }

    constexpr fn operator * (SVector3 lhs, float scalar) noex -> SVector3
    {
        return { lhs.x * scalar, lhs.y * scalar };
    }

    constexpr fn operator / (SVector3 lhs, float scalar) noex -> SVector3
    {
        return { lhs.x / scalar, lhs.y / scalar };
    }

    constexpr fn operator += (SVector3& lhs, const SVector3& rhs) noex -> SVector3&
    {
        lhs.x += rhs.x;
        lhs.y += rhs.y;
        lhs.z += rhs.z;
        return lhs;
    }

    constexpr fn operator -= (SVector3& lhs, const SVector3& rhs) noex -> SVector3&
    {
        lhs.x -= rhs.x;
        lhs.y -= rhs.y;
        lhs.z -= rhs.z;
        return lhs;
    }

    constexpr fn operator *= (SVector3& lhs, float scalar) noex -> SVector3&
    {
        lhs.x *= scalar;
        lhs.y *= scalar;
        lhs.z *= scalar;
        return lhs;
    }

    constexpr fn operator /= (SVector3& lhs, float scalar) noex -> SVector3&
    {
        lhs.x /= scalar;
        lhs.y /= scalar;
        lhs.z /= scalar;
        return lhs;
    }

    constexpr fn operator == (SVector3 lhs, SVector3 rhs) noex -> bool
    {
        return Math::IsApproximatelyZero(lhs.x, rhs.x) &&
            Math::IsApproximatelyZero(lhs.y, rhs.y) &&
            Math::IsApproximatelyZero(lhs.z, rhs.z);
    }

    constexpr fn operator != (SVector3 lhs, SVector3 rhs) noex -> bool
    {
        return !(lhs == rhs);
    }

    constexpr fn operator > (SVector3 lhs, SVector3 rhs) noex -> bool
    {
        return (lhs.x > rhs.x) && (lhs.y > rhs.y) && (lhs.z > rhs.z);
    }

    constexpr fn operator < (SVector3 lhs, SVector3 rhs) noex -> bool
    {
        return (lhs.x < rhs.x) && (lhs.y < rhs.y) && (lhs.z < rhs.z);
    }

    constexpr fn operator >= (SVector3 lhs, SVector3 rhs) noex -> bool
    {
        return (lhs.x >= rhs.x) && (lhs.y >= rhs.y) && (lhs.z >= rhs.z);
    }

    constexpr fn operator <= (SVector3 lhs, SVector3 rhs) noex -> bool
    {
        return (lhs.x <= rhs.x) && (lhs.y <= rhs.y) && (lhs.z <= rhs.z);
    }
}