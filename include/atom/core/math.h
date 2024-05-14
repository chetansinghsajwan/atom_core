#pragma once

// #include <limits>
// #include <algorithm>

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

    template <typename num_t>
    constexpr auto min_of(num_t num0, num_t num1) -> num_t
    {
        return num0 < num1 ? num0 : num1;
    }
}
