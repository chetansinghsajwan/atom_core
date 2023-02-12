#pragma once
#include "Atom/Logging/LogMsg.h"

namespace Atom::Logging
{
    /// --------------------------------------------------------------------------------------------
    /// Abstract class defining a log target (like a file, console, network).
    /// --------------------------------------------------------------------------------------------
    class LogTarget
    {
        /// ----------------------------------------------------------------------------------------
        /// Writes the message into underlying target.
        /// ----------------------------------------------------------------------------------------
        virtual void Write(const LogMsg& logMsg) = 0;

        /// ----------------------------------------------------------------------------------------
        /// If storing messages in a buffer, flushes it.
        /// ----------------------------------------------------------------------------------------
        virtual void Flush() = 0;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ptr type used to manage LogTarget in this logging api. It's recommended to use this 
    /// to type to store LogTarget instance. This keeps compatibility with the logging api.
    /// --------------------------------------------------------------------------------------------
    using LogTargetPtr = SharedPtr<LogTarget>;
}