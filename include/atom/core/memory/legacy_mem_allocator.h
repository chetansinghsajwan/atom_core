#pragma once
// #include "atom/core/_std.h"
#include "atom/core/core.h"

namespace atom
{
    class legacy_mem_allocator
    {
    public:
        auto alloc(usize size) -> void*
        {
            return std::malloc(size);
        }

        auto realloc(void* mem, usize size) -> void*
        {
            return std::realloc(mem, size);
        }

        auto dealloc(void* mem)
        {
            std::free(mem);
        }
    };
}
