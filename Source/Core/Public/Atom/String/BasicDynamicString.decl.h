#pragma once
#include "_StringImpl.decl.h"
#include "Atom/Containers/DynamicArray.decl.h"

namespace Atom
{
    template <typename TEncoding, typename TAlloc = DefaultMemAllocator>
    using BasicDynamicString = _StringImpl
    <
        TEncoding,
        DynamicArray<BasicChar<TEncoding>, TAlloc>
    >;
}