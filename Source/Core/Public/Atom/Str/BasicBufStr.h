#pragma once
#include "Atom/Containers/BufArr.h"
#include "_StrImpl.h"

namespace Atom
{
    template <typename TEncoding, usize BufSize, typename TAlloc = DefaultMemAllocator>
    using BasicBufStr = _StrImpl<TEncoding, BufArr<BasicChar<TEncoding>, BufSize, TAlloc>>;
}
