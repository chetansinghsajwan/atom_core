#pragma once

// #include <random>

namespace atom
{
    using mt19937 = ::std::mt19937;

    template <typename tint>
    using uniform_int_distribution = ::std::uniform_int_distribution<tint>;

    using random_device = std::random_device;
}
