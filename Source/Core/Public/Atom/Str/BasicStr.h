#pragma once
#include "BasicBufStr.h"

namespace Atom
{
    template <typename TEncoding, typename TAlloc = DefaultMemAllocator>
    using BasicStr = BasicBufStr<TEncoding, 24, TAlloc>;
}
