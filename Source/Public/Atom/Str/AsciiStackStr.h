#pragma once
#include "Atom/Text/AsciiEncoding.h"
#include "BasicStackStr.h"

namespace Atom
{
    template <usize Size>
    using AsciiStackStr = BasicStackStr<AsciiEncoding, Size>;
}
