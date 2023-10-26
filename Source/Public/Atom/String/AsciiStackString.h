#pragma once
#include "Atom/Text/AsciiEncoding.h"
#include "BasicStackString.h"

namespace Atom
{
    template <usize Size>
    using AsciiStackString = BasicStackString<AsciiEncoding, Size>;
}
