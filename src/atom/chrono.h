#pragma once
// #include <chrono>

#include "atom/core.h"

namespace atom
{
    using time_point = std::chrono::system_clock::time_point;

    namespace time
    {
        inline auto now()
        {
            return std::chrono::system_clock::now();
        }
    }
}
