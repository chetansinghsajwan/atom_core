#pragma once
#include "Atom/Core.h"

#include <memory>

namespace Atom
{
    class LegacyMemAllocator
    {
    public:
        auto Alloc(usize size) -> MutMemPtr<void>
        {
            return std::malloc(size);
        }

        auto Realloc(MutMemPtr<void> mem, usize size) -> MutMemPtr<void>
        {
            return std::realloc(mem.unwrap(), size.unwrap());
        }

        auto Dealloc(MutMemPtr<void> mem)
        {
            std::free(mem.unwrap());
        }
    };
}
