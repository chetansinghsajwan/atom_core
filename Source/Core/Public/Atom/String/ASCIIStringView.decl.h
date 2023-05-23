#pragma once
#include "BasicStringView.decl.h"
#include "Atom/String/ASCIICharEncoding.h"

namespace Atom
{
    using ASCIIStringView = BasicStringView<ASCIICharEncoding>;
}