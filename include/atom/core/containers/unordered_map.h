#pragma once
#include "atom/core/_std.h"

namespace atom
{
    template <typename key_type, typename elem_type>
    using unordered_map = std::unordered_map<key_type, elem_type>;
}
