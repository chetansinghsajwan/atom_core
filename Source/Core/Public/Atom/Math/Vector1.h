#pragma once
#include "Atom/Math/Core.h"

namespace Atom
{
    class SVector1
    {
    public:
        f32 x;
    };

    constexpr auto operator+(SVector1 lhs, SVector1 rhs) -> SVector1
    {
        return { lhs.x + rhs.x };
    }

    constexpr auto operator-(SVector1 lhs, SVector1 rhs) -> SVector1
    {
        return { lhs.x - rhs.x };
    }

    constexpr auto operator*(SVector1 lhs, f32 scalar) -> SVector1
    {
        return { lhs.x * scalar };
    }

    constexpr auto operator/(SVector1 lhs, f32 scalar) -> SVector1
    {
        return { lhs.x / scalar };
    }

    constexpr auto operator+=(SVector1& lhs, const SVector1& rhs) -> SVector1&
    {
        lhs.x += rhs.x;
        return lhs;
    }

    constexpr auto operator-=(SVector1& lhs, const SVector1& rhs) -> SVector1&
    {
        lhs.x -= rhs.x;
        return lhs;
    }

    constexpr auto operator*=(SVector1& lhs, f32 scalar) -> SVector1&
    {
        lhs.x *= scalar;
        return lhs;
    }

    constexpr auto operator/=(SVector1& lhs, f32 scalar) -> SVector1&
    {
        lhs.x /= scalar;
        return lhs;
    }

    constexpr auto operator==(SVector1 lhs, SVector1 rhs) -> bool
    {
        return Math::IsApproximatelyZero(lhs.x, rhs.x);
    }

    constexpr auto operator!=(SVector1 lhs, SVector1 rhs) -> bool
    {
        return !(lhs == rhs);
    }

    constexpr auto operator>(SVector1 lhs, SVector1 rhs) -> bool
    {
        return lhs.x > rhs.x;
    }

    constexpr auto operator<(SVector1 lhs, SVector1 rhs) -> bool
    {
        return lhs.x < rhs.x;
    }

    constexpr auto operator>=(SVector1 lhs, SVector1 rhs) -> bool
    {
        return lhs.x >= rhs.x;
    }

    constexpr auto operator<=(SVector1 lhs, SVector1 rhs) -> bool
    {
        return lhs.x <= rhs.x;
    }
}
