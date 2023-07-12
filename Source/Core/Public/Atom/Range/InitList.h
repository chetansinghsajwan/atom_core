#pragma once
#include <initializer_list>

#include "Atom/Core.h"

namespace Atom
{
    template <tname T>
    using InitList = std::initializer_list<T>;
}