#pragma once
#include "atom/core/_std.h"

namespace atom
{
    template <typename key_t, typename elem_t>
    using unordered_map = std::unordered_map<key_t, elem_t>;
}
