#pragma once
#include "Atom/Containers/StaticArr.h"
#include "_StrImpl.h"

namespace Atom
{
    // TODO: Fix this, cannot inherit to preserve aggregate initialization.
    template <typename TEncoding, usize Size>
    using BasicStaticStr = StaticArr<BasicChar<TEncoding>, Size>;
    // using BasicStaticStr = _StrImpl
    // <
    //     TEncoding,
    //     StaticArr<BasicChar<TEncoding>, Size>
    // >;
}
