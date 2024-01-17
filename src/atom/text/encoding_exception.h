#pragma once
#include "atom/core/exceptions.h"

namespace atom::text
{
    /// --------------------------------------------------------------------------------------------
    /// exception representing error during processing character encoding.
    /// --------------------------------------------------------------------------------------------
    class encoding_exception: public exception
    {
    public:
        encoding_exception(string msg)
            : exception(mov(msg))
        {}
    };
}
