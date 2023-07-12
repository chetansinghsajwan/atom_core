#pragma once
#include "BasicBufStr.decl.h"

namespace Atom
{
    template <tname TEncoding, tname TAlloc = DefaultMemAllocator>
    using BasicStr = BasicBufStr<TEncoding, 24, TAlloc>;
}