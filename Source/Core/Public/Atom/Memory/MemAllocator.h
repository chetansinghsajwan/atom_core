#pragma once
#include "Atom/Core.h"
#include "Atom/Exceptions.h"
#include "Atom/TTI.h"

namespace Atom
{
    struct LegacyMemAllocator
    {
        void* Alloc(usize size)
        {
            return std::malloc(size);
        }

        void* Realloc(void* mem, usize size)
        {
            return std::realloc(mem, size);
        }

        void Dealloc(void* mem)
        {
            std::free(mem);
        }
    };

    using DefaultMemAllocator = LegacyMemAllocator;
}