#pragma once
#include <unordered_map>

#include "Atom/Core.h"
#include "Atom/Containers/Core.h"

namespace Atom
{
    template <typename TKey, typename TElement>
    using UnorderedMap = std::unordered_map<TKey, TElement>;
}