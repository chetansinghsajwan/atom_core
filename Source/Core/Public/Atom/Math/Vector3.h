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

    constexpr SVector3 operator + (SVector3 lhs, SVector3 rhs) noex
    {
        return { lhs.x + rhs.x, lhs.y + rhs.y };
    }

    constexpr SVector3 operator - (SVector3 lhs, SVector3 rhs) noex
    {
        return { lhs.x - rhs.x, lhs.y - rhs.y };
    }

    constexpr SVector3 operator * (SVector3 lhs, float scalar) noex
    {
        return { lhs.x * scalar, lhs.y * scalar };
    }

    constexpr SVector3 operator / (SVector3 lhs, float scalar) noex
    {
        return { lhs.x / scalar, lhs.y / scalar };
    }

    constexpr SVector3& operator += (SVector3& lhs, const SVector3& rhs) noex
    {
        lhs.x += rhs.x;
        lhs.y += rhs.y;
        lhs.z += rhs.z;
        return lhs;
    }

    constexpr SVector3& operator -= (SVector3& lhs, const SVector3& rhs) noex
    {
        lhs.x -= rhs.x;
        lhs.y -= rhs.y;
        lhs.z -= rhs.z;
        return lhs;
    }

    constexpr SVector3& operator *= (SVector3& lhs, float scalar) noex
    {
        lhs.x *= scalar;
        lhs.y *= scalar;
        lhs.z *= scalar;
        return lhs;
    }

    constexpr SVector3& operator /= (SVector3& lhs, float scalar) noex
    {
        lhs.x /= scalar;
        lhs.y /= scalar;
        lhs.z /= scalar;
        return lhs;
    }

    constexpr bool operator == (SVector3 lhs, SVector3 rhs) noex
    {
        return Math::IsApproximatelyZero(lhs.x, rhs.x) &&
            Math::IsApproximatelyZero(lhs.y, rhs.y) &&
            Math::IsApproximatelyZero(lhs.z, rhs.z);
    }

    constexpr bool operator != (SVector3 lhs, SVector3 rhs) noex
    {
        return !(lhs == rhs);
    }

    constexpr bool operator > (SVector3 lhs, SVector3 rhs) noex
    {
        return (lhs.x > rhs.x) && (lhs.y > rhs.y) && (lhs.z > rhs.z);
    }

    constexpr bool operator < (SVector3 lhs, SVector3 rhs) noex
    {
        return (lhs.x < rhs.x) && (lhs.y < rhs.y) && (lhs.z < rhs.z);
    }

    constexpr bool operator >= (SVector3 lhs, SVector3 rhs) noex
    {
        return (lhs.x >= rhs.x) && (lhs.y >= rhs.y) && (lhs.z >= rhs.z);
    }

    constexpr bool operator <= (SVector3 lhs, SVector3 rhs) noex
    {
        return (lhs.x <= rhs.x) && (lhs.y <= rhs.y) && (lhs.z <= rhs.z);
    }
}