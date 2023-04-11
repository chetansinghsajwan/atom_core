#pragma once
#include "Atom/Core.h"
#include "Atom/String.h"

namespace Atom
{
    class Exception
    {
    public:
        Exception(String msg) noexcept:
            m_msg{ msg } { }

    public:
        StringView GetMsg() noexcept
        {
            return m_msg;
        }

    protected:
        String m_msg;
    };

    using RuntimeException = Exception;
    using InvalidOperationException = Exception;
    using InvalidArgumentException = Exception;
    using NullPointerException = Exception;
}