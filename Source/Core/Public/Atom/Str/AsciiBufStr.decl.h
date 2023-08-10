#pragma once
#include "Atom/Text/AsciiEncoding.decl.h"
#include "BasicBufStr.decl.h"

namespace Atom
{
    template <usize BufSize>
    using AsciiBufStr = BasicBufStr<AsciiEncoding, BufSize>;
}
