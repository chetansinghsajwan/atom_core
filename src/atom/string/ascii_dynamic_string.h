#pragma once
#include "atom/text/ascii_encoding.h"
#include "basic_dynamic_string.h"

namespace atom
{
    using ascii_dynamic_string = basic_dynamic_string<ascii_encoding>;
}
