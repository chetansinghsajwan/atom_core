#pragma once
#include "_StrImpl.decl.h"
#include "Atom/Containers/DynArr.decl.h"

namespace Atom
{
    template <tname TEncoding, tname TAlloc = DefaultMemAllocator>
    using BasicDynamicStr = _StrImpl
    <
        TEncoding,
        DynArr<BasicChar<TEncoding>, TAlloc>
    >;
}