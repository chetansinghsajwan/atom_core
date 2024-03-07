#pragma once

#include "limits"
#include "algorithm"
#include <algorithm>

namespace atom::math
{
    template <typename num_type>
    constexpr auto max() -> num_type
    {
        return std::numeric_limits<num_type>::max();
    }

    template <typename num_type>
    constexpr auto abs(num_type num) -> num_type
    {
        return std::abs(num);
    }

    template <typename num_type>
    constexpr auto clamp(num_type num, num_type low, num_type high) -> num_type
    {
        return std::clamp(num, low, high);
    }
}
