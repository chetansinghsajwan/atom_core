#pragma once
#include <array>

#include "Atom/Containers/Insertable.h"

namespace Atom
{
    template <typename T, usize Size>
    using StaticArray = ::std::array<T, Size>;
}