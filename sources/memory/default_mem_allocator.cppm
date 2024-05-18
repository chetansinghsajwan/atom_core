export module atom.core:default_mem_allocator;

import :legacy_mem_allocator;

namespace atom
{
    export using default_mem_allocator = legacy_mem_allocator;
}
