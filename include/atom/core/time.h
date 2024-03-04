#pragma once
#include "atom/core/_std.h"

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
