#pragma once
#include "basic_buf_string.h"

namespace atom
{
    template <typename tencoding, typename allocator_type = default_mem_allocator>
    using basic_string = basic_buf_string<tencoding, 24, allocator_type>;
}
