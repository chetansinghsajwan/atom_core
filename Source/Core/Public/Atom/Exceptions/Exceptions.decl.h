#pragma once
#include <source_location>

#include "Atom/String/String.decl.h"
#include "Atom/String/StringView.decl.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// Base class for all exceptions.
    /// --------------------------------------------------------------------------------------------
    class Exception
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// Basic constructor with msg.
        /// ----------------------------------------------------------------------------------------
        Exception(String msg) noexcept;

    public:
        /// ----------------------------------------------------------------------------------------
        /// Get message explaining the exception.
        /// ----------------------------------------------------------------------------------------
        StringView GetMsg() const noexcept;

    protected:
        String m_msg;
    };

    using RuntimeException = Exception;
    using InvalidOperationException = Exception;
    using InvalidArgumentException = Exception;
    using NullPointerException = Exception;
    using OutOfRangeException = Exception;
}

/// ------------------------------------------------------------------------------------------------
/// ATOM_THROW
/// ------------------------------------------------------------------------------------------------
#define ATOM_THROW throw