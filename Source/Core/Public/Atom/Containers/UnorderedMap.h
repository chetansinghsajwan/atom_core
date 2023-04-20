#pragma once
#include <unordered_map>

#include "Atom/Containers/Insertable.h"

namespace Atom
{
    template <typename TKey, typename TElement>
    using UnorderedMap = ::std::unordered_map<TKey, TElement>;
}