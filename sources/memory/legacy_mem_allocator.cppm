export module atom.core:legacy_mem_allocator;

import std;
import :core;

namespace atom
{
    export class legacy_mem_allocator
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
