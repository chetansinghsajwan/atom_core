#pragma once
#include "BasicBufStr.decl.h"
#include "Atom/Text/AsciiEncoding.decl.h"

namespace Atom
{
    template <usize BufSize>
    using AsciiBufStr = BasicBufStr<AsciiEncoding, BufSize>;
}