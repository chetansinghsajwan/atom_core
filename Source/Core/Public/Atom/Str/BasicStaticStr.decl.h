#pragma once
#include "_StrImpl.decl.h"
#include "Atom/Containers/StaArr.decl.h"

namespace Atom
{
    // TODO: Fix this, cannot inherit to preserve aggregate initialization.
    template <tname TEncoding, usize Size>
    using BasicStaticStr = StaArr<BasicChar<TEncoding>, Size>;
    // using BasicStaticStr = _StrImpl
    // <
    //     TEncoding,
    //     StaArr<BasicChar<TEncoding>, Size>
    // >;
}