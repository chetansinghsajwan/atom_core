#pragma once
#include "atom/math/core.h"

namespace atom
{
    class svector1
    {
    public:
        f32 x;
    };

    constexpr auto operator+(svector1 lhs, svector1 rhs) -> svector1
    {
        return { lhs.x + rhs.x };
    }

    constexpr auto operator-(svector1 lhs, svector1 rhs) -> svector1
    {
        return { lhs.x - rhs.x };
    }

    constexpr auto operator*(svector1 lhs, f32 scalar) -> svector1
    {
        return { lhs.x * scalar };
    }

    constexpr auto operator/(svector1 lhs, f32 scalar) -> svector1
    {
        return { lhs.x / scalar };
    }

    constexpr auto operator+=(svector1& lhs, const svector1& rhs) -> svector1&
    {
        lhs.x += rhs.x;
        return lhs;
    }

    constexpr auto operator-=(svector1& lhs, const svector1& rhs) -> svector1&
    {
        lhs.x -= rhs.x;
        return lhs;
    }

    constexpr auto operator*=(svector1& lhs, f32 scalar) -> svector1&
    {
        lhs.x *= scalar;
        return lhs;
    }

    constexpr auto operator/=(svector1& lhs, f32 scalar) -> svector1&
    {
        lhs.x /= scalar;
        return lhs;
    }

    constexpr auto operator==(svector1 lhs, svector1 rhs) -> bool
    {
        return lhs.x.sub(rhs.x).eq_zero_approx();
    }

    constexpr auto operator!=(svector1 lhs, svector1 rhs) -> bool
    {
        return !(lhs == rhs);
    }

    constexpr auto operator>(svector1 lhs, svector1 rhs) -> bool
    {
        return lhs.x > rhs.x;
    }

    constexpr auto operator<(svector1 lhs, svector1 rhs) -> bool
    {
        return lhs.x < rhs.x;
    }

    constexpr auto operator>=(svector1 lhs, svector1 rhs) -> bool
    {
        return lhs.x >= rhs.x;
    }

    constexpr auto operator<=(svector1 lhs, svector1 rhs) -> bool
    {
        return lhs.x <= rhs.x;
    }
}
