#pragma once
#include "atom/core.h"

namespace atom
{
    template <typename tchar_encoding>
    using basic_char = typename tchar_encoding::tchar;
}
