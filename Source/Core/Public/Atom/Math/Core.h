#pragma once
#include <cmath>
#include <numeric>

#include "Atom/Core.h"

namespace Atom
{
    template <typename T>
    using NumLimits = std::numeric_limits<T>;
}

namespace Atom::Math
{
    constexpr double Abs(double value) noexcept
    {
        // TODO: Improve this, maybe using sign bit flag.
        return value * (value < 0 ? -1 : 1);
    }

    constexpr bool IsApproximatelyZero(double lhs, double rhs) noexcept
    {
        return Abs(lhs - rhs) < 0.0001f;
    }

    template <typename TInt>
    constexpr TInt Clamp(const TInt& lhs, const TInt& rhs)
    {
        return lhs < rhs ? lhs : rhs;
    }
}