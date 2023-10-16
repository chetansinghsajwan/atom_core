#pragma once
#include "BasicStackStr.h"
#include "DefaultEncoding.h"

namespace Atom
{
    template <usize size>
    using StackStr = BasicStackStr<CharEncoding, size>;
}
