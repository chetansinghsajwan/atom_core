#pragma once
#include "BasicBufString.h"

namespace Atom
{
    template <typename TEncoding, typename TAlloc = DefaultMemAllocator>
    using BasicString = BasicBufString<TEncoding, 24, TAlloc>;
}
