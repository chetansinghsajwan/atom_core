#pragma once
#include "atom/containers/dynamic_array.h"
#include "_string_impl.h"

namespace atom
{
    template <typename encoding_type, typename allocator_type = default_mem_allocator>
    using basic_dynamic_string = _string_impl<encoding_type, dynamic_array<basic_char<encoding_type>, allocator_type>>;
}
