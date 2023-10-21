#pragma once
#include "Atom/Containers/DynArr.h"
#include "_StrImpl.h"

namespace Atom
{
    template <typename TEncoding, typename TAlloc = DefaultMemAllocator>
    using BasicDynamicStr = _StrImpl<TEncoding, DynArr<BasicChar<TEncoding>, TAlloc>>;
}
