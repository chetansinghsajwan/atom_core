#pragma once
#include <initializer_list>

#include "Atom/Core.h"

namespace Atom
{
    template <typename TElement>
    using InitializerList = std::initializer_list<TElement>;
}