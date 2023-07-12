#pragma once
#include <atomic>

#include "Atom/Core.h"

namespace Atom
{
    template <tname T>
    using Atomic = std::atomic<T>;
}