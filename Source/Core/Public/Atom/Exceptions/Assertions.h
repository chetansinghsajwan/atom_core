#pragma once
#include "Atom/Exceptions/Exceptions.h"
#include "Atom/Exceptions/Assertions.decl.h"

namespace Atom
{
    inline ctor AssertException::AssertException(Str msg, StrView assertExpr):
        assertExpr{ assertExpr }, Exception(mov(msg)) { }
}