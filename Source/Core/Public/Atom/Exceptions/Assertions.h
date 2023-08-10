#pragma once
#include "Atom/Exceptions/Assertions.decl.h"
#include "Atom/Exceptions/Exceptions.h"

namespace Atom
{
    inline AssertException::AssertException(Str msg, StrView assertExpr)
        : assertExpr{ assertExpr }
        , Exception(mov(msg))
    {}
}
