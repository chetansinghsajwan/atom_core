#pragma once
#include "BasicStaticString.decl.h"
#include "DefaultEncoding.decl.h"

namespace Atom
{
    template <usize Size>
    using StaticString = BasicStaticString<CharEncoding, Size>;
}