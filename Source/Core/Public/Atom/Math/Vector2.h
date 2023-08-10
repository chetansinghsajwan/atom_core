#pragma once
#include "Atom/Math/Core.h"

namespace Atom
{
    class SVector2
    {
    public:
        f32 x;
        f32 y;
    };

    constexpr fn operator+(SVector2 lhs, SVector2 rhs) -> SVector2
    {
        return { lhs.x + rhs.x, lhs.y + rhs.y };
    }

    constexpr fn operator-(SVector2 lhs, SVector2 rhs) -> SVector2
    {
        return { lhs.x - rhs.x, lhs.y - rhs.y };
    }

    constexpr fn operator*(SVector2 lhs, f32 scalar) -> SVector2
    {
        return { lhs.x * scalar, lhs.y * scalar };
    }

    constexpr fn operator/(SVector2 lhs, f32 scalar) -> SVector2
    {
        return { lhs.x / scalar, lhs.y / scalar };
    }

    constexpr fn operator+=(SVector2& lhs, const SVector2& rhs) -> SVector2&
    {
        lhs.x += rhs.x;
        lhs.y += rhs.y;
        return lhs;
    }

    constexpr fn operator-=(SVector2& lhs, const SVector2& rhs) -> SVector2&
    {
        lhs.x -= rhs.x;
        lhs.y -= rhs.y;
        return lhs;
    }

    constexpr fn operator*=(SVector2& lhs, f32 scalar) -> SVector2&
    {
        lhs.x *= scalar;
        lhs.y *= scalar;
        return lhs;
    }

    constexpr fn operator/=(SVector2& lhs, f32 scalar) -> SVector2&
    {
        lhs.x /= scalar;
        lhs.y /= scalar;
        return lhs;
    }

    constexpr fn operator==(SVector2 lhs, SVector2 rhs) -> bool
    {
        return Math::IsApproximatelyZero(lhs.x, rhs.x) &&
            Math::IsApproximatelyZero(lhs.y, rhs.y);
    }

    constexpr fn operator!=(SVector2 lhs, SVector2 rhs) -> bool
    {
        return !(lhs == rhs);
    }

    constexpr fn operator>(SVector2 lhs, SVector2 rhs) -> bool
    {
        return (lhs.x > rhs.x) && (lhs.y > rhs.y);
    }

    constexpr fn operator<(SVector2 lhs, SVector2 rhs) -> bool
    {
        return (lhs.x < rhs.x) && (lhs.y < rhs.y);
    }

    constexpr fn operator>=(SVector2 lhs, SVector2 rhs) -> bool
    {
        return (lhs.x >= rhs.x) && (lhs.y >= rhs.y);
    }

    constexpr fn operator<=(SVector2 lhs, SVector2 rhs) -> bool
    {
        return (lhs.x <= rhs.x) && (lhs.y <= rhs.y);
    }
}