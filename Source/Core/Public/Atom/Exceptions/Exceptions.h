#pragma once
#include <stdexcept>

#include "Atom/Core.h"
#include "Atom/String.h"

namespace Atom
{
    class Exception
    {
    public:
        Exception(String msg) noexcept:
            m_msg{ msg } { }

    protected:
        String m_msg;
    };

    // using Exception = std::exception;
    using RuntimeError = Exception;
    using InvalidOperationException = Exception;
    using InvalidArgumentException = Exception;
    using NullPointerException = Exception;
    using AssertionException = Exception;
}