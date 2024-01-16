#pragma once
// #include <random>

namespace Atom
{
    using MT19937 = ::std::mt19937;

    template <typename TInt>
    using UniformIntDistribution = ::std::uniform_int_distribution<TInt>;

    using RandomDevice = std::random_device;
}
