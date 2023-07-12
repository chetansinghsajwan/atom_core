#pragma once
#include "InsertableReq.h"

#include <unordered_map>

namespace Atom
{
    template <tname TKey, tname TElement>
    using UnorderedMap = ::std::unordered_map<TKey, TElement>;
}