#pragma once
#include "Atom/Containers/BufArray.h"
#include "_StringImpl.h"

namespace Atom
{
    template <typename TEncoding, usize BufSize, typename TAlloc = DefaultMemAllocator>
    using BasicBufString = _StringImpl<TEncoding, BufArray<BasicChar<TEncoding>, BufSize, TAlloc>>;
}
