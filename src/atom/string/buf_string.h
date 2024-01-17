#pragma once
#include "basic_buf_string.h"
#include "default_encoding.h"

namespace atom
{
    template <usize buf_size>
    using buf_string = basic_buf_string<char_encoding, buf_size>;
}
