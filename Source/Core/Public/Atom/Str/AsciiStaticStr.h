#pragma once
#include "Atom/Text/AsciiEncoding.h"
#include "BasicStaticStr.h"

namespace Atom
{
    template <usize Size>
    using AsciiStaticStr = BasicStaticStr<AsciiEncoding, Size>;
}
