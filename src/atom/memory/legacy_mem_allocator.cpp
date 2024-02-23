export module atom.core:memory.legacy_mem_allocator;
import :std;
import :core;
import :mem_ptr;

namespace atom
{
    export class legacy_mem_allocator
    {
    public:
        auto alloc(usize size) -> mut_mem_ptr<void>
        {
            return std::malloc(size.to_unwrapped());
        }

        auto realloc(mut_mem_ptr<void> mem, usize size) -> mut_mem_ptr<void>
        {
            return std::realloc(mem.to_unwrapped(), size.to_unwrapped());
        }

        auto dealloc(mut_mem_ptr<void> mem)
        {
            std::free(mem.to_unwrapped());
        }
    };
}
