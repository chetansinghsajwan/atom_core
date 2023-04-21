#pragma once
#include "Atom/String/String.decl.h"
#include "Atom/String/StringView.decl.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    class Exception
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        Exception(String msg) noexcept;

    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        StringView GetMsg() const noexcept;

    protected:
        String m_msg;
    };

    using RuntimeException = Exception;
    using InvalidOperationException = Exception;
    using InvalidArgumentException = Exception;
    using NullPointerException = Exception;
}