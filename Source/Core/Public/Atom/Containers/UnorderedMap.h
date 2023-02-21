#pragma once
#include <unordered_map>

#include "Atom/Core.h"
#include "Atom/Containers/Core.h"

namespace Atom
{
    template <typename K, typename T>
    using UnorderedMap = std::unordered_map<K, T>;
}