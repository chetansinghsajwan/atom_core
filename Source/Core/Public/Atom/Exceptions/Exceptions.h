#pragma once
#include <stdexcept>

#include "Atom/Core.h"

namespace Atom
{
    using Exception = std::exception;
    using InvalidOperationException = std::runtime_error;
    using InvalidArgumentException = std::runtime_error;
    using NullPointerException = std::runtime_error;
    using AssertionException = std::runtime_error;
}