#pragma once
#include "Atom/Math/Core.h"

namespace Atom
{
    struct Vector3
    {
        float x;
        float y;
        float z;
    };

    constexpr Vector3 operator + (Vector3 lhs, Vector3 rhs) noexcept
    {
        return { lhs.x + rhs.x, lhs.y + rhs.y };
    }

    constexpr Vector3 operator - (Vector3 lhs, Vector3 rhs) noexcept
    {
        return { lhs.x - rhs.x, lhs.y - rhs.y };
    }

    constexpr Vector3 operator * (Vector3 lhs, float scalar) noexcept
    {
        return { lhs.x * scalar, lhs.y * scalar };
    }

    constexpr Vector3 operator / (Vector3 lhs, float scalar) noexcept
    {
        return { lhs.x / scalar, lhs.y / scalar };
    }

    constexpr Vector3& operator += (Vector3& lhs, const Vector3& rhs) noexcept
    {
        lhs.x += rhs.x;
        lhs.y += rhs.y;
        return lhs;
    }

    constexpr Vector3& operator -= (Vector3& lhs, const Vector3& rhs) noexcept
    {
        lhs.x -= rhs.x;
        lhs.y -= rhs.y;
        return lhs;
    }

    constexpr Vector3& operator *= (Vector3& lhs, float scalar) noexcept
    {
        lhs.x *= scalar;
        lhs.y *= scalar;
        return lhs;
    }

    constexpr Vector3& operator /= (Vector3& lhs, float scalar) noexcept
    {
        lhs.x /= scalar;
        lhs.y /= scalar;
        return lhs;
    }

    constexpr bool operator == (Vector3 lhs, Vector3 rhs) noexcept
    {
        return Math::IsApproximatelyZero(lhs.x, rhs.x) &&
            Math::IsApproximatelyZero(lhs.y, rhs.y) &&
            Math::IsApproximatelyZero(lhs.z, rhs.z);
    }

    constexpr bool operator != (Vector3 lhs, Vector3 rhs) noexcept
    {
        return !(lhs == rhs);
    }

    constexpr bool operator > (Vector3 lhs, Vector3 rhs) noexcept
    {
        return (lhs.x > rhs.x) && (lhs.y > rhs.y);
    }

    constexpr bool operator < (Vector3 lhs, Vector3 rhs) noexcept
    {
        return (lhs.x < rhs.x) && (lhs.y < rhs.y);
    }

    constexpr bool operator >= (Vector3 lhs, Vector3 rhs) noexcept
    {
        return (lhs.x >= rhs.x) && (lhs.y >= rhs.y);
    }

    constexpr bool operator <= (Vector3 lhs, Vector3 rhs) noexcept
    {
        return (lhs.x <= rhs.x) && (lhs.y <= rhs.y);
    }
}