#pragma once
#include "BasicDynamicStr.decl.h"
#include "Atom/Text/AsciiEncoding.decl.h"

namespace Atom
{
    using AsciiDynamicStr = BasicDynamicStr<AsciiEncoding>;
}