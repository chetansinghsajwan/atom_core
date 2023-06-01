#pragma once
#include "BasicStringView.decl.h"
#include "Atom/Text/AsciiEncoding.decl.h"

namespace Atom
{
    using AsciiStringView = BasicStringView<AsciiEncoding>;
}