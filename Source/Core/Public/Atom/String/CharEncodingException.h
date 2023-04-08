#pragma once
#include "Atom/Exceptions.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// Exception representing error during processing character encoding.
    /// --------------------------------------------------------------------------------------------
    class CharEncodingException: public Exception
    {
    public:
        CharEncodingException(String in_msg) noexcept:
            Exception(MOVE(in_msg)) { }
    };
}