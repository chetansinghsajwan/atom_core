#pragma once
#include "Atom/Containers/ArrView.decl.h"
#include "_StrImpl.decl.h"

namespace Atom
{
    template <typename TEncoding>
    using BasicStrView = ArrView<BasicChar<TEncoding>>;
}
