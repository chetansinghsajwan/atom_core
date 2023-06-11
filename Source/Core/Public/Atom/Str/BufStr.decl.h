#pragma once
#include "BasicBufStr.decl.h"
#include "DefaultEncoding.decl.h"

namespace Atom
{
    template <usize BufSize>
    using BufStr = BasicBufStr<CharEncoding, BufSize>;
}