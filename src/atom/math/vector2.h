#pragma once
#include "atom/math/core.h"

namespace atom
{
    class svector2
    {
    public:
        f32 x;
        f32 y;
    };

    constexpr auto operator+(svector2 lhs, svector2 rhs) -> svector2
    {
        return { lhs.x + rhs.x, lhs.y + rhs.y };
    }

    constexpr auto operator-(svector2 lhs, svector2 rhs) -> svector2
    {
        return { lhs.x - rhs.x, lhs.y - rhs.y };
    }

    constexpr auto operator*(svector2 lhs, f32 scalar) -> svector2
    {
        return { lhs.x * scalar, lhs.y * scalar };
    }

    constexpr auto operator/(svector2 lhs, f32 scalar) -> svector2
    {
        return { lhs.x / scalar, lhs.y / scalar };
    }

    constexpr auto operator+=(svector2& lhs, const svector2& rhs) -> svector2&
    {
        lhs.x += rhs.x;
        lhs.y += rhs.y;
        return lhs;
    }

    constexpr auto operator-=(svector2& lhs, const svector2& rhs) -> svector2&
    {
        lhs.x -= rhs.x;
        lhs.y -= rhs.y;
        return lhs;
    }

    constexpr auto operator*=(svector2& lhs, f32 scalar) -> svector2&
    {
        lhs.x *= scalar;
        lhs.y *= scalar;
        return lhs;
    }

    constexpr auto operator/=(svector2& lhs, f32 scalar) -> svector2&
    {
        lhs.x /= scalar;
        lhs.y /= scalar;
        return lhs;
    }

    constexpr auto operator==(svector2 lhs, svector2 rhs) -> bool
    {
        return lhs.x.sub(rhs.x).eq_zero_approx() && lhs.y.sub(rhs.y).eq_zero_approx();
    }

    constexpr auto operator!=(svector2 lhs, svector2 rhs) -> bool
    {
        return !(lhs == rhs);
    }

    constexpr auto operator>(svector2 lhs, svector2 rhs) -> bool
    {
        return (lhs.x > rhs.x) && (lhs.y > rhs.y);
    }

    constexpr auto operator<(svector2 lhs, svector2 rhs) -> bool
    {
        return (lhs.x < rhs.x) && (lhs.y < rhs.y);
    }

    constexpr auto operator>=(svector2 lhs, svector2 rhs) -> bool
    {
        return (lhs.x >= rhs.x) && (lhs.y >= rhs.y);
    }

    constexpr auto operator<=(svector2 lhs, svector2 rhs) -> bool
    {
        return (lhs.x <= rhs.x) && (lhs.y <= rhs.y);
    }
}
