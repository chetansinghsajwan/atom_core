#pragma once
#include "atom/text/ascii_encoding.h"
#include "basic_stack_string.h"

namespace atom
{
    template <usize size>
    using ascii_stack_string = basic_stack_string<ascii_encoding, size>;
}
