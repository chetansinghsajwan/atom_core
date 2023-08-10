#pragma once
#include "Atom/Containers/DynArr.decl.h"
#include "_StrImpl.decl.h"

namespace Atom
{
    template <typename TEncoding, typename TAlloc = DefaultMemAllocator>
    using BasicDynamicStr = _StrImpl<TEncoding, DynArr<BasicChar<TEncoding>, TAlloc>>;
}
