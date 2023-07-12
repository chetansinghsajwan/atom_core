#pragma once
#include <chrono>

#include "Atom/Core.h"

namespace Atom
{
    using TimePoint = std::chrono::system_clock::time_point;

    namespace Time
    {
        inline fn Now()
        {
            return std::chrono::system_clock::now();
        }
    }
}