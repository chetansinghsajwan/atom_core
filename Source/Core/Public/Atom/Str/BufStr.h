#pragma once
#include "BasicBufStr.h"
#include "DefaultEncoding.h"

namespace Atom
{
    template <usize BufSize>
    using BufStr = BasicBufStr<CharEncoding, BufSize>;
}
