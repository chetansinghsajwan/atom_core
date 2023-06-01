#pragma once
#include "_StringImpl.decl.h"
#include "Atom/Containers/StaticArray.decl.h"

namespace Atom
{
    // TODO: Fix this, cannot inherit to preserve aggregate initialization.
    template <typename TEncoding, usize Size>
    using BasicStaticString = StaticArray<BasicChar<TEncoding>, Size>;
    // using BasicStaticString = _StringImpl
    // <
    //     TEncoding,
    //     StaticArray<BasicChar<TEncoding>, Size>
    // >;
}