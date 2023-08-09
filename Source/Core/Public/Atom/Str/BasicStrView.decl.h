#pragma once
#include "_StrImpl.decl.h"
#include "Atom/Containers/ArrView.decl.h"

namespace Atom
{
    template <typename TEncoding>
    using BasicStrView = ArrView<BasicChar<TEncoding>>;
}