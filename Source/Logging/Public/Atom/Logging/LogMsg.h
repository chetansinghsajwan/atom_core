#pragma once
#include "Atom/Logging/Internal/Core.h"
#include "fmt/format.h"

namespace Atom::Logging
{
    /// --------------------------------------------------------------------------------------------
    /// LogLevel is used to define the level of the log.
    /// --------------------------------------------------------------------------------------------
    enum class LogLevel: byte
    {
        Trace,
        Debug,
        Info,
        Warn,
        Error,
        Fatal,
        OFF         ///< Used to indicate logging is off.
    };

    enum class color { red, green, blue };

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

namespace fmt
{
    template <>
    struct formatter<Atom::Logging::LogLevel>: formatter<Atom::StringView>
    {
        template <typename FormatContext>
        auto format(Atom::Logging::LogLevel c, FormatContext& ctx) const
        {
            string_view name = "UNKNOWN";
            switch (c)
            {
                case Atom::Logging::LogLevel::Trace: name = "TRACE"; break;
                case Atom::Logging::LogLevel::Debug: name = "DEBUG"; break;
                case Atom::Logging::LogLevel::Info: name = "INFO"; break;
                case Atom::Logging::LogLevel::Warn: name = "WARN"; break;
                case Atom::Logging::LogLevel::Error: name = "ERROR"; break;
                case Atom::Logging::LogLevel::Fatal: name = "FATAL"; break;
                case Atom::Logging::LogLevel::OFF: name = "OFF"; break;
            }

            return formatter<string_view>::format(name, ctx);
        }
    };
}
