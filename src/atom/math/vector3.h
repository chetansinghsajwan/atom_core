#pragma once
#include "atom/math/core.h"

namespace atom
{
    class svector3
    {
    public:
        f32 x;
        f32 y;
        f32 z;
    };

    constexpr auto operator+(svector3 lhs, svector3 rhs) -> svector3
    {
        return { lhs.x + rhs.x, lhs.y + rhs.y };
    }

    constexpr auto operator-(svector3 lhs, svector3 rhs) -> svector3
    {
        return { lhs.x - rhs.x, lhs.y - rhs.y };
    }

    constexpr auto operator*(svector3 lhs, f32 scalar) -> svector3
    {
        return { lhs.x * scalar, lhs.y * scalar };
    }

    constexpr auto operator/(svector3 lhs, f32 scalar) -> svector3
    {
        return { lhs.x / scalar, lhs.y / scalar };
    }

    constexpr auto operator+=(svector3& lhs, const svector3& rhs) -> svector3&
    {
        lhs.x += rhs.x;
        lhs.y += rhs.y;
        lhs.z += rhs.z;
        return lhs;
    }

    constexpr auto operator-=(svector3& lhs, const svector3& rhs) -> svector3&
    {
        lhs.x -= rhs.x;
        lhs.y -= rhs.y;
        lhs.z -= rhs.z;
        return lhs;
    }

    constexpr auto operator*=(svector3& lhs, f32 scalar) -> svector3&
    {
        lhs.x *= scalar;
        lhs.y *= scalar;
        lhs.z *= scalar;
        return lhs;
    }

    constexpr auto operator/=(svector3& lhs, f32 scalar) -> svector3&
    {
        lhs.x /= scalar;
        lhs.y /= scalar;
        lhs.z /= scalar;
        return lhs;
    }

    constexpr auto operator==(svector3 lhs, svector3 rhs) -> bool
    {
        return lhs.x.sub(rhs.x).eq_zero_approx() && lhs.y.sub(rhs.y).eq_zero_approx()
               && lhs.z.sub(rhs.z).eq_zero_approx();
    }

    constexpr auto operator!=(svector3 lhs, svector3 rhs) -> bool
    {
        return !(lhs == rhs);
    }

    constexpr auto operator>(svector3 lhs, svector3 rhs) -> bool
    {
        return (lhs.x > rhs.x) && (lhs.y > rhs.y) && (lhs.z > rhs.z);
    }

    constexpr auto operator<(svector3 lhs, svector3 rhs) -> bool
    {
        return (lhs.x < rhs.x) && (lhs.y < rhs.y) && (lhs.z < rhs.z);
    }

    constexpr auto operator>=(svector3 lhs, svector3 rhs) -> bool
    {
        return (lhs.x >= rhs.x) && (lhs.y >= rhs.y) && (lhs.z >= rhs.z);
    }

    constexpr auto operator<=(svector3 lhs, svector3 rhs) -> bool
    {
        return (lhs.x <= rhs.x) && (lhs.y <= rhs.y) && (lhs.z <= rhs.z);
    }
}
