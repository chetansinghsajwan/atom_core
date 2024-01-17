#pragma once
#include "basic_string_view.h"
#include "default_encoding.h"

namespace atom
{
    using string_view = basic_string_view<char_encoding>;
}
