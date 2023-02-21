#pragma once
#include "Atom/Core.h"
#include <atomic>

namespace Atom
{
    template <typename T>
    using Atomic = std::atomic<T>;
}