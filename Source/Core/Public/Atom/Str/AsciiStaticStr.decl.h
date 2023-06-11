#pragma once
#include "BasicStaticStr.decl.h"
#include "Atom/Text/AsciiEncoding.decl.h"

namespace Atom
{
    template <usize Size>
    using AsciiStaticStr = BasicStaticStr<AsciiEncoding, Size>;
}