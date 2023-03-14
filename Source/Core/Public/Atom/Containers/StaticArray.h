#pragma once
#include <array>

#include "Atom/Core.h"

namespace Atom
{
    template <typename TElement, SizeT Size>
    using StaticArray = ::std::array<TElement, Size>;
}