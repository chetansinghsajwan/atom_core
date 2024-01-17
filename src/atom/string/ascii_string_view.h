#pragma once
#include "atom/text/ascii_encoding.h"
#include "basic_string_view.h"

namespace atom
{
    using ascii_string_view = basic_string_view<ascii_encoding>;
}
