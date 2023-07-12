#pragma once
#include "_StrImpl.decl.h"
#include "Atom/Containers/BufArr.decl.h"

namespace Atom
{
    template <tname TEncoding, usize BufSize, tname TAlloc = DefaultMemAllocator>
    using BasicBufStr = _StrImpl
    <
        TEncoding,
        BufArr<BasicChar<TEncoding>, BufSize, TAlloc>
    >;
}