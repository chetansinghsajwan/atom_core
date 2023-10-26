#pragma once
#include "BasicBufString.h"
#include "DefaultEncoding.h"

namespace Atom
{
    template <usize BufSize>
    using BufString = BasicBufString<CharEncoding, BufSize>;
}
