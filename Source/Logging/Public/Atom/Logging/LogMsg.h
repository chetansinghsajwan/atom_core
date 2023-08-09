#pragma once
#include "fmt/chrono.h" // TODO: Remove this

#include "Internal/Core.h"

#include "Atom/Chrono.h"
#include "Atom/Str.h"


namespace Atom::Logging
{
    /// --------------------------------------------------------------------------------------------
    /// ELogLevel is used to define the level of the log.
    /// --------------------------------------------------------------------------------------------
    enum class ELogLevel: byte
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
    class LogMsg
    {
        /// ----------------------------------------------------------------------------------------
        /// Message of the log.
        /// ----------------------------------------------------------------------------------------
        pub StrView msg;

        /// ----------------------------------------------------------------------------------------
        /// Name of the logger through which this message was logged.
        /// ----------------------------------------------------------------------------------------
        pub StrView loggerName;

        /// ----------------------------------------------------------------------------------------
        /// Level of this message.
        /// ----------------------------------------------------------------------------------------
        pub ELogLevel lvl;

        /// ----------------------------------------------------------------------------------------
        /// Time when this message was logged.
        /// 
        /// This is necessary to store as the message may be logged asynchronously or may face 
        /// some latency in writing.
        /// ----------------------------------------------------------------------------------------
        pub TimePoint time;
    };
}

namespace Atom
{
    template < >
    class StrViewConverter<Logging::ELogLevel>
    {
        pub cexpr fn Convert(Logging::ELogLevel lvl) -> StrView
        {
            switch (lvl)
            {
                case Logging::ELogLevel::Trace : return "Trace"; break;
                case Logging::ELogLevel::Debug : return "Debug"; break;
                case Logging::ELogLevel::Info  : return "Info";  break;
                case Logging::ELogLevel::Warn  : return "Warn";  break;
                case Logging::ELogLevel::Error : return "Error"; break;
                case Logging::ELogLevel::Fatal : return "Fatal"; break;
                case Logging::ELogLevel::OFF   : return "OFF";   break;
                default                        : return "UNKNOWN";
            }
        }
    };

    static_assert(RStrFmtArgFmtable<Logging::ELogLevel>, "ELogLevel is not formattable.");
}
