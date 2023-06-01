#pragma once
#include "BasicStaticString.decl.h"
#include "Atom/Text/AsciiEncoding.decl.h"

namespace Atom
{
    template <usize Size>
    using AsciiStaticString = BasicStaticString<AsciiEncoding, Size>;
}