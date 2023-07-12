#pragma once
#include <random>

namespace Atom
{
    using MT19937 = ::std::mt19937;

    template <tname TInt>
    using UniformIntDistribution = ::std::uniform_int_distribution<TInt>;

    using RandomDevice = std::random_device;
}