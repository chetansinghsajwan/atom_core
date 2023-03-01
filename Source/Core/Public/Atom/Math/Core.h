#pragma once
#include <cmath>
#include "Atom/Core.h"

namespace Atom::Math
{
    constexpr bool IsApproximatelyZero(float lhs, float rhs) noexcept
    {
        return std::fabs(lhs - rhs) < 0.0001f;
    }
}