#pragma once
#include <array>

#include "Atom/Core.h"

namespace Atom
{
    template <typename TElement, usize Size>
    using StaticArray = ::std::array<TElement, Size>;
}