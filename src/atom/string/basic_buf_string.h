#pragma once
#include "atom/containers/buf_array.h"
#include "_string_impl.h"

namespace atom
{
    template <typename tencoding, usize buf_size, typename allocator_type = default_mem_allocator>
    using basic_buf_string = _string_impl<tencoding, buf_array<basic_char<tencoding>, buf_size, allocator_type>>;
}
