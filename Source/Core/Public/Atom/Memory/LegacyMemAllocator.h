#pragma once
#include "Atom/Core.h"

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
}