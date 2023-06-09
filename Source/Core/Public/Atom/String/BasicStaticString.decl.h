#pragma once
#include "_StringImpl.decl.h"
#include "Atom/Containers/StaArr.decl.h"

namespace Atom
{
    // TODO: Fix this, cannot inherit to preserve aggregate initialization.
    template <typename TEncoding, usize Size>
    using BasicStaticString = StaArr<BasicChar<TEncoding>, Size>;
    // using BasicStaticString = _StringImpl
    // <
    //     TEncoding,
    //     StaArr<BasicChar<TEncoding>, Size>
    // >;
}