#pragma once
#include "Atom/Core.h"

namespace Atom
{
    template <typename TCharEncoding>
    using BasicChar = typename TCharEncoding::TChar;
}