#pragma once

// #include "atom/memory/default_mem_allocator.h"

namespace atom
{
    class shared_ptr_default_allocator;

    template <typename type>
    class shared_ptr_default_destroyer;

    template <typename type>
    class shared_ptr;
}
