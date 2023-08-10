#pragma once
#include <atomic>

#include "Atom/Core.h"

namespace Atom
{
    template <typename T>
    using Atomic = std::atomic<T>;
}
