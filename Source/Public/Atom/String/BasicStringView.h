#pragma once
#include "Atom/Containers/ArrayView.h"
#include "_StringImpl.h"

namespace Atom
{
    template <typename TEncoding>
    using BasicStringView = ArrayView<BasicChar<TEncoding>>;
}
