#pragma once
#include "Atom/Text/AsciiEncoding.h"
#include "BasicBufStr.h"

namespace Atom
{
    template <usize BufSize>
    using AsciiBufStr = BasicBufStr<AsciiEncoding, BufSize>;
}
