#pragma once
#include "_StrImpl.decl.h"
#include "Atom/Containers/BufArr.decl.h"

namespace Atom
{
    template <typename TEncoding, usize BufSize, typename TAlloc = DefaultMemAllocator>
    using BasicBufStr = _StrImpl
    <
        TEncoding,
        BufArr<BasicChar<TEncoding>, BufSize, TAlloc>
    >;
}