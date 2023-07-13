#pragma once
#include "Atom/Core.h"

namespace Atom
{
    class LegacyMemAllocator
    {
        pub fn Alloc(usize size) -> void*
        {
            return std::malloc(size);
        }

        pub fn Realloc(void* mem, usize size) -> void*
        {
            return std::realloc(mem, size);
        }

        pub fn Dealloc(void* mem)
        {
            std::free(mem);
        }
    };
}