#pragma once
#include "Atom/Core.h"

namespace Atom
{
    class LegacyMemAllocator
    {
    public:
        fn Alloc(usize size) -> void*
        {
            return std::malloc(size);
        }

        fn Realloc(void* mem, usize size) -> void*
        {
            return std::realloc(mem, size);
        }

        fn Dealloc(void* mem)
        {
            std::free(mem);
        }
    };
}