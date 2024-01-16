#pragma once
#include "InsertableReq.h"

// #include <unordered_map>

namespace Atom
{
    template <typename TKey, typename TElement>
    using UnorderedMap = ::std::unordered_map<TKey, TElement>;
}
