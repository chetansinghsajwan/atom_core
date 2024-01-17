#pragma once
#include "atom/containers/dynamic_array.h"
#include "_string_impl.h"

namespace atom
{
    template <typename tencoding, typename allocator_type = default_mem_allocator>
    using basic_dynamic_string = _string_impl<tencoding, dynamic_array<basic_char<tencoding>, allocator_type>>;
}
