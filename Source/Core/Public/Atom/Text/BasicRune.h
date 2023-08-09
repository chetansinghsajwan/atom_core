#pragma once
#include "Atom/Core.h"

namespace Atom
{
    template <typename TCharEncoding>
    using BasicRune = typename TCharEncoding::TRune;
}