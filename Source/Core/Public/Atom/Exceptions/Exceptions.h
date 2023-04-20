#pragma once
#include "Atom/Core.h"

namespace Atom
{
    class Exception
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// @TODO Resolve circular dependencies.
        /// ----------------------------------------------------------------------------------------
        Exception(const char16* msg) noexcept { }

    public:
        const char16* GetMsg() noexcept
        {
            return u"";
        }
    };

    using RuntimeException = Exception;
    using InvalidOperationException = Exception;
    using InvalidArgumentException = Exception;
    using NullPointerException = Exception;
}