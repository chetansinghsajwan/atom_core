#pragma once
#include "BasicStaticStr.decl.h"
#include "DefaultEncoding.decl.h"

namespace Atom
{
    template <usize Size>
    using StaticStr = BasicStaticStr<CharEncoding, Size>;
}