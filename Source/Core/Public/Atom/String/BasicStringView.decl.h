#pragma once
#include "_StringImpl.decl.h"
#include "Atom/Containers/ArrayView.decl.h"

namespace Atom
{
    template <typename TEncoding>
    using BasicStringView = _StringImpl
    <
        TEncoding,
        ArrayView<BasicChar<TEncoding>>
    >;
}