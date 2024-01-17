#pragma once
#include "Atom/Memory/DefaultMemAllocator.h"

namespace Atom
{
    class SharedPtrDefaultAllocator;

    template <typename T>
    class SharedPtrDefaultDestroyer;

    template <typename T>
    class SharedPtr;
}
