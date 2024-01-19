#pragma once
#include "basic_buf_string.h"

namespace atom
{
    template <typename encoding_type, typename allocator_type = default_mem_allocator>
    using basic_string = basic_buf_string<encoding_type, 24, allocator_type>;
}
