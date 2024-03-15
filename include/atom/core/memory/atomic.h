#pragma once
#include "atom/core/_std.h"

namespace atom
{
    template <typename value_t>
    using atomic = std::atomic<value_t>;
}
