export module atom.core:memory.default_mem_allocator;
import :memory.legacy_mem_allocator;

namespace atom
{
    export using default_mem_allocator = legacy_mem_allocator;
}
