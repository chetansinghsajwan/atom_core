#pragma once
#include "Atom/Logging/Internal/Core.h"

namespace Atom::Logging
{
    /// --------------------------------------------------------------------------------------------
    /// LogLevel is used to define the level of the log.
    /// --------------------------------------------------------------------------------------------
    enum class LogLevel : byte
    {
        Trace,
        Debug,
        Info,
        Warn,
        Error,
        Fatal,
        OFF         ///< Used to indicate logging is off.
    };

    /// --------------------------------------------------------------------------------------------
    /// Lightweight object containing all the details of the log.
    /// --------------------------------------------------------------------------------------------
    struct LogMsg
    {
        /// Message of the log.
        StringView msg;

        /// Name of the logger through which this message was logged.
        StringView loggerName;

        /// Level of this message.
        LogLevel lvl;

        /// Time when this message was logged.
        /// 
        /// This is necessary to store as the message may be logged asynchronously or may face 
        /// some latency in writing.
        TimePoint time;
    };
}