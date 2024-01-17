#pragma once
#include "atom/containers/static_array.h"
#include "_string_impl.h"

namespace atom
{
    // todo: fix this, cannot inherit to preserve aggregate initialization.
    template <typename tencoding, usize size>
    using basic_stack_string = static_array<basic_char<tencoding>, size>;
    // using basic_stack_string = _string_impl
    // <
    //     tencoding,
    //     static_array<basic_char<tencoding>, size>
    // >;
}
