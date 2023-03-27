#pragma once
#include "Atom/Core.h"

namespace Atom
{
    template <typename TElement>
    using ConstArrayIterator = const TElement*;

    template <typename TElement>
    using ArrayIterator = TElement*;
}