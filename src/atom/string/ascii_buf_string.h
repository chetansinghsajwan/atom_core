#pragma once
#include "atom/text/ascii_encoding.h"
#include "basic_buf_string.h"

namespace atom
{
    template <usize buf_size>
    using ascii_buf_string = basic_buf_string<ascii_encoding, buf_size>;
}
