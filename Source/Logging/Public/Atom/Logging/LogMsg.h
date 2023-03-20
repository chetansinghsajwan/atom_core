#pragma once
#include "fmt/chrono.h" // TODO: Remove this

#include "Atom/Chrono.h"
#include "Atom/String.h"
#include "Atom/Fmt.h"

#include "Atom/Logging/Internal/Core.h"

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

    /// --------------------------------------------------------------------------------------------
    /// Lightweight object containing all the details of the log.
    /// --------------------------------------------------------------------------------------------
    struct LogMsg
    {
        /// ----------------------------------------------------------------------------------------
        /// Message of the log.
        /// ----------------------------------------------------------------------------------------
        StringView msg;

        /// ----------------------------------------------------------------------------------------
        /// Name of the logger through which this message was logged.
        /// ----------------------------------------------------------------------------------------
        StringView loggerName;

        /// ----------------------------------------------------------------------------------------
        /// Level of this message.
        /// ----------------------------------------------------------------------------------------
        LogLevel lvl;

        /// ----------------------------------------------------------------------------------------
        /// Time when this message was logged.
        /// 
        /// This is necessary to store as the message may be logged asynchronously or may face 
        /// some latency in writing.
        /// ----------------------------------------------------------------------------------------
        TimePoint time;
    };
}

namespace fmt
{
    template < >
    struct formatter<Atom::StringView, Atom::Char>:
        public formatter<basic_string_view<Atom::Char>, Atom::Char>
    {
        using _fmtStringFormatter = formatter<basic_string_view<Atom::Char>, Atom::Char>;
        using _fmtString = basic_string_view<Atom::Char>;

        template <typename TFormatContext>
        auto format(Atom::StringView str, TFormatContext& ctx) const
        {
            return _fmtStringFormatter::format(_fmtString{ str.data(), str.size() }, ctx);
        }
    };

    template < >
    struct formatter<Atom::Logging::LogLevel, Atom::Char>:
        public formatter<Atom::StringView, Atom::Char>
    {
        template <typename TFormatContext>
        auto format(Atom::Logging::LogLevel lvl, TFormatContext& ctx) const
        {
            Atom::StringView name = TEXT("UNKNOWN");
            switch (lvl)
            {
                case Atom::Logging::LogLevel::Trace: name = TEXT("TRACE"); break;
                case Atom::Logging::LogLevel::Debug: name = TEXT("DEBUG"); break;
                case Atom::Logging::LogLevel::Info:  name = TEXT("INFO");  break;
                case Atom::Logging::LogLevel::Warn:  name = TEXT("WARN");  break;
                case Atom::Logging::LogLevel::Error: name = TEXT("ERROR"); break;
                case Atom::Logging::LogLevel::Fatal: name = TEXT("FATAL"); break;
                case Atom::Logging::LogLevel::OFF:   name = TEXT("OFF");   break;
            }

            return formatter<Atom::StringView, Atom::Char>::format(name, ctx);
        }
    };
}