#pragma once
#include "Atom/Text/AsciiEncoding.h"
#include "BasicBufString.h"

namespace Atom
{
    template <usize BufSize>
    using AsciiBufString = BasicBufString<AsciiEncoding, BufSize>;
}
