#pragma once
#include <cmath>
#include "Atom/Core.h"

namespace Atom::Math
{
    constexpr float Abs(float value) noexcept
    {
        return value * (value < 0 ? -1 : 1);
    }

    constexpr bool IsApproximatelyZero(float lhs, float rhs) noexcept
    {
        return Abs(lhs - rhs) < 0.0001f;
    }
}