#pragma once
#include "BasicStaticStr.h"
#include "DefaultEncoding.h"

namespace Atom
{
    template <usize Size>
    using StaticStr = BasicStaticStr<CharEncoding, Size>;
}
