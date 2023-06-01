#pragma once
#include "BasicBufString.decl.h"
#include "DefaultEncoding.decl.h"

namespace Atom
{
    template <usize BufSize>
    using BufString = BasicBufString<CharEncoding, BufSize>;
}