#pragma once
#include "BasicStrView.decl.h"
#include "Atom/Text/AsciiEncoding.decl.h"

namespace Atom
{
    using AsciiStrView = BasicStrView<AsciiEncoding>;
}