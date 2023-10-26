#pragma once
#include "Atom/Containers/DynamicArray.h"
#include "_StringImpl.h"

namespace Atom
{
    template <typename TEncoding, typename TAlloc = DefaultMemAllocator>
    using BasicDynamicString = _StringImpl<TEncoding, DynamicArray<BasicChar<TEncoding>, TAlloc>>;
}
