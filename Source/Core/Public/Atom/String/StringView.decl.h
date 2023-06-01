#pragma once
#include "BasicStringView.decl.h"
#include "DefaultEncoding.decl.h"

namespace Atom
{
    using StringView = BasicStringView<CharEncoding>;
}