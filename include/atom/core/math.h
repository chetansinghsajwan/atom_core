#pragma once

#include "limits"
#include "algorithm"
#include <algorithm>

namespace atom::math
{
    template <typename num_t>
    constexpr auto min() -> num_t
    {
        return std::numeric_limits<num_t>::min();
    }

    template <typename num_t>
    constexpr auto max() -> num_t
    {
        return std::numeric_limits<num_t>::max();
    }

    template <typename num_t>
    constexpr auto abs(num_t num) -> num_t
    {
        return std::abs(num);
    }

    template <typename num_t>
    constexpr auto clamp(num_t num, num_t low, num_t high) -> num_t
    {
        return std::clamp(num, low, high);
    }
}
