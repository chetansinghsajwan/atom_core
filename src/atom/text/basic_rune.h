#pragma once
#include "atom/core.h"

namespace atom
{
    template <typename tchar_encoding>
    using basic_rune = typename tchar_encoding::rune_type;
}
