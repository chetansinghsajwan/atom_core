#pragma once
#include "EncodingException.decl.h"
#include "Atom/Core/Exceptions/Exception.h"

namespace Atom::Text
{
    EncodingException::EncodingException(String msg) noexcept:
        Exception(MOVE(msg)) { }
}