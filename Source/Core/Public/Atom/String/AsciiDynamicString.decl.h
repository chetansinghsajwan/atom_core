#pragma once
#include "BasicDynamicString.decl.h"
#include "Atom/Text/AsciiEncoding.decl.h"

namespace Atom
{
    using AsciiDynamicString = BasicDynamicString<AsciiEncoding>;
}