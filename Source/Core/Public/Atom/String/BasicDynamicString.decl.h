#pragma once
#include "_StringImpl.decl.h"
#include "Atom/Containers/DynArr.decl.h"

namespace Atom
{
    template <typename TEncoding, typename TAlloc = DefaultMemAllocator>
    using BasicDynamicString = _StringImpl
    <
        TEncoding,
        DynArr<BasicChar<TEncoding>, TAlloc>
    >;
}