#pragma once
#include "Atom/Containers/ArrView.h"
#include "_StrImpl.h"

namespace Atom
{
    template <typename TEncoding>
    using BasicStrView = ArrView<BasicChar<TEncoding>>;
}
