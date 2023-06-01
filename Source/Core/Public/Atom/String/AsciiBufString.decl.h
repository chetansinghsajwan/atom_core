#pragma once
#include "BasicBufString.decl.h"
#include "Atom/Text/AsciiEncoding.decl.h"

namespace Atom
{
    template <usize BufSize>
    using AsciiBufString = BasicBufString<AsciiEncoding, BufSize>;
}