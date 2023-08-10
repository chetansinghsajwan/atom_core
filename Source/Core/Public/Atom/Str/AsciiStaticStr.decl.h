#pragma once
#include "Atom/Text/AsciiEncoding.decl.h"
#include "BasicStaticStr.decl.h"

namespace Atom
{
    template <usize Size>
    using AsciiStaticStr = BasicStaticStr<AsciiEncoding, Size>;
}
