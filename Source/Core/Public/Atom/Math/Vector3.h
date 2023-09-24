#pragma once
#include "Atom/Math/Core.h"

namespace Atom
{
    class SVector3
    {
    public:
        f32 x;
        f32 y;
        f32 z;
    };

    constexpr auto operator+(SVector3 lhs, SVector3 rhs) -> SVector3
    {
        return { lhs.x + rhs.x, lhs.y + rhs.y };
    }

    constexpr auto operator-(SVector3 lhs, SVector3 rhs) -> SVector3
    {
        return { lhs.x - rhs.x, lhs.y - rhs.y };
    }

    constexpr auto operator*(SVector3 lhs, f32 scalar) -> SVector3
    {
        return { lhs.x * scalar, lhs.y * scalar };
    }

    constexpr auto operator/(SVector3 lhs, f32 scalar) -> SVector3
    {
        return { lhs.x / scalar, lhs.y / scalar };
    }

    constexpr auto operator+=(SVector3& lhs, const SVector3& rhs) -> SVector3&
    {
        lhs.x += rhs.x;
        lhs.y += rhs.y;
        lhs.z += rhs.z;
        return lhs;
    }

    constexpr auto operator-=(SVector3& lhs, const SVector3& rhs) -> SVector3&
    {
        lhs.x -= rhs.x;
        lhs.y -= rhs.y;
        lhs.z -= rhs.z;
        return lhs;
    }

    constexpr auto operator*=(SVector3& lhs, f32 scalar) -> SVector3&
    {
        lhs.x *= scalar;
        lhs.y *= scalar;
        lhs.z *= scalar;
        return lhs;
    }

    constexpr auto operator/=(SVector3& lhs, f32 scalar) -> SVector3&
    {
        lhs.x /= scalar;
        lhs.y /= scalar;
        lhs.z /= scalar;
        return lhs;
    }

    constexpr auto operator==(SVector3 lhs, SVector3 rhs) -> bool
    {
        return lhs.x.sub(rhs.x).eqZeroApprox() &&
            lhs.y.sub(rhs.y).eqZeroApprox() &&
            lhs.z.sub(rhs.z).eqZeroApprox();
    }

    constexpr auto operator!=(SVector3 lhs, SVector3 rhs) -> bool
    {
        return !(lhs == rhs);
    }

    constexpr auto operator>(SVector3 lhs, SVector3 rhs) -> bool
    {
        return (lhs.x > rhs.x) && (lhs.y > rhs.y) && (lhs.z > rhs.z);
    }

    constexpr auto operator<(SVector3 lhs, SVector3 rhs) -> bool
    {
        return (lhs.x < rhs.x) && (lhs.y < rhs.y) && (lhs.z < rhs.z);
    }

    constexpr auto operator>=(SVector3 lhs, SVector3 rhs) -> bool
    {
        return (lhs.x >= rhs.x) && (lhs.y >= rhs.y) && (lhs.z >= rhs.z);
    }

    constexpr auto operator<=(SVector3 lhs, SVector3 rhs) -> bool
    {
        return (lhs.x <= rhs.x) && (lhs.y <= rhs.y) && (lhs.z <= rhs.z);
    }
}
