#pragma once
#include "insertable_req.h"

// #include <unordered_map>

namespace atom
{
    template <typename tkey, typename telement>
    using unordered_map = ::std::unordered_map<tkey, telement>;
}
