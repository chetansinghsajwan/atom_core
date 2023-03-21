#pragma once
#include "fmt/chrono.h" // TODO: Remove this

#include "Atom/Chrono.h"
#include "Atom/String.h"
#include "Atom/Fmt.h"

#include "Atom/Logging/Internal/Core.h"

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
        ELogLevel lvl;

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
    struct formatter<Atom::Logging::ELogLevel, Atom::Char>:
        public formatter<Atom::StringView, Atom::Char>
    {
        template <typename TFormatContext>
        auto format(Atom::Logging::ELogLevel lvl, TFormatContext& ctx) const
        {
            Atom::StringView name = TEXT("UNKNOWN");
            switch (lvl)
            {
                case Atom::Logging::ELogLevel::Trace: name = TEXT("TRACE"); break;
                case Atom::Logging::ELogLevel::Debug: name = TEXT("DEBUG"); break;
                case Atom::Logging::ELogLevel::Info:  name = TEXT("INFO");  break;
                case Atom::Logging::ELogLevel::Warn:  name = TEXT("WARN");  break;
                case Atom::Logging::ELogLevel::Error: name = TEXT("ERROR"); break;
                case Atom::Logging::ELogLevel::Fatal: name = TEXT("FATAL"); break;
                case Atom::Logging::ELogLevel::OFF:   name = TEXT("OFF");   break;
            }

            return formatter<Atom::StringView, Atom::Char>::format(name, ctx);
        }
    };
}