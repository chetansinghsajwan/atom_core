#pragma once
#include "_StringImpl.decl.h"
#include "Atom/Containers/ArrView.decl.h"

namespace Atom
{
    template <typename TEncoding>
    using BasicStringView = _StringImpl
    <
        TEncoding,
        ArrView<BasicChar<TEncoding>>
    >;
}