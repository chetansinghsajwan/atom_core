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
            return std::malloc(size.unwrap());
        }

        auto realloc(mut_mem_ptr<void> mem, usize size) -> mut_mem_ptr<void>
        {
            return std::realloc(mem.unwrap(), size.unwrap());
        }

        auto dealloc(mut_mem_ptr<void> mem)
        {
            std::free(mem.unwrap());
        }
    };
}
