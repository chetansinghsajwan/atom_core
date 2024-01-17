#pragma once
#include "Atom/Containers/StaticArray.h"
#include "_StringImpl.h"

namespace Atom
{
    // TODO: Fix this, cannot inherit to preserve aggregate initialization.
    template <typename TEncoding, usize Size>
    using BasicStackString = StaticArray<BasicChar<TEncoding>, Size>;
    // using BasicStackString = _StringImpl
    // <
    //     TEncoding,
    //     StaticArray<BasicChar<TEncoding>, Size>
    // >;
}
