#pragma once
#include <initializer_list>

#include "Atom/Core.h"

namespace Atom
{
    template <typename T>
    using InitList = std::initializer_list<T>;
}