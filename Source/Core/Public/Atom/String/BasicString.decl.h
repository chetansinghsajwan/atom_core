#pragma once
#include "BasicBufString.decl.h"
#include "DefaultEncoding.decl.h"

namespace Atom
{
    template <typename TEncoding, typename TAlloc = DefaultMemAllocator>
    using BasicString = BasicBufString<TEncoding, 24, TAlloc>;
}