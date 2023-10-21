#pragma once
#include "Atom/Core.h"

#include <memory>

namespace Atom
{
    class LegacyMemAllocator
    {
    public:
        auto Alloc(usize size) -> void*
        {
            return std::malloc(size);
        }

        auto Realloc(void* mem, usize size) -> void*
        {
            return std::realloc(mem, size);
        }

        auto Dealloc(void* mem)
        {
            std::free(mem);
        }
    };
}
