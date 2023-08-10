#pragma once
#include "Atom/Core/Exceptions/Exception.h"
#include "EncodingException.decl.h"

namespace Atom::Text
{
    EncodingException::EncodingException(DynStr msg)
        : Exception(mov(msg))
    {}
}
