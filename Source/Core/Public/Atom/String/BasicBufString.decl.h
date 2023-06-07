#pragma once
#include "_StringImpl.decl.h"
#include "Atom/Containers/BufArray.decl.h"

namespace Atom
{
    template <typename TEncoding, usize BufSize, typename TAlloc = DefaultMemAllocator>
    using BasicBufString = _StringImpl
    <
        TEncoding,
        BufArr<BasicChar<TEncoding>, BufSize, TAlloc>
    >;
}