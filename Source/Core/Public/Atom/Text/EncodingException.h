#pragma once
#include "EncodingException.decl.h"
#include "Atom/Core/Exceptions/Exception.h"

namespace Atom::Text
{
    EncodingException::EncodingException(DynStr msg) noex:
        Exception(MOVE(msg)) { }
}