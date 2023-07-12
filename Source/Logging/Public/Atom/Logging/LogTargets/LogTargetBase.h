#pragma once
#include "Atom/Logging/LogTarget.h"

namespace Atom::Logging::Internal
{
    /// --------------------------------------------------------------------------------------------
    /// Base class for LogTargets with base functionality like formatting, thread safety and
    /// level filtering.
    /// 
    /// @TODO Add thread safety.
    /// @TODO Make default log and flush level global.
    /// --------------------------------------------------------------------------------------------
    class LogTargetBase: public LogTarget
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// Default constructs LogTargetBase().
        /// ----------------------------------------------------------------------------------------
        ctor LogTargetBase() noex:
            m_logLevel(ELogLevel::Debug), m_flushLevel(ELogLevel::Info),
            m_hasWritten(false), m_alwaysFlush(false) { }

    //// -------------------------------------------------------------------------------------------
    //// API Functions
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// Filters and Formats the LogMsg and passes it to write.
        /// 
        /// @EXCEPTION_SAFETY Strong.
        /// 
        /// @SEE m_Write().
        /// ----------------------------------------------------------------------------------------
        virtual fn Write(const LogMsg& in_logMsg) -> void ofinal
        {
            if (CheckLogLevel(in_logMsg.lvl))
            {
                // TODO: Add chrono support for Atom.Fmt.
                // Str result = StrFmter().Fmt(TEXT("[{}] [{}] {}: {}\n"),
                //     in_logMsg.time, in_logMsg.lvl, in_logMsg.loggerName, in_logMsg.msg);

                Str result = StrFmter().Fmt(TEXT("[{}] {}: {}\n"),
                    in_logMsg.lvl, in_logMsg.loggerName, in_logMsg.msg);

                m_hasWritten = true;
                m_Write(in_logMsg, result);

                if (CheckFlushLevel(in_logMsg.lvl))
                {
                    m_Flush();
                }
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// Flushes if {ShouldFlush() == true}.
        /// 
        /// @EXCEPTION_SAFETY Strong.
        /// ----------------------------------------------------------------------------------------
        virtual fn Flush() -> void ofinal
        {
            if (ShouldFlush())
            {
                m_Flush();
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// Get the log level.
        /// ----------------------------------------------------------------------------------------
        fn GetLogLevel() const noex -> ELogLevel
        {
            return m_logLevel;
        }

        /// ----------------------------------------------------------------------------------------
        /// Sets the log level.
        /// ----------------------------------------------------------------------------------------
        fn SetLogLevel(ELogLevel in_lvl) noex
        {
            m_logLevel = in_lvl;
        }

        /// ----------------------------------------------------------------------------------------
        /// Checks if we should log the message of specified level.
        /// ----------------------------------------------------------------------------------------
        fn CheckLogLevel(ELogLevel in_lvl) const noex -> bool
        {
            if (in_lvl == ELogLevel::OFF) return false;
            if (in_lvl < m_logLevel) return false;

            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// Gets the flush level.
        /// ----------------------------------------------------------------------------------------
        fn GetFlushLevel() const noex -> ELogLevel
        {
            return m_flushLevel;
        }

        /// ----------------------------------------------------------------------------------------
        /// Sets the flush level.
        /// ----------------------------------------------------------------------------------------
        fn SetFlushLevel(ELogLevel in_lvl) noex
        {
            m_flushLevel = in_lvl;
        }

        /// ----------------------------------------------------------------------------------------
        /// Checks if should flush after logging the message of specified level.
        /// It also asks ShouldFlush().
        /// ----------------------------------------------------------------------------------------
        fn CheckFlushLevel(ELogLevel in_lvl) const noex -> bool
        {
            if (!m_hasWritten) return false;
            if (in_lvl == ELogLevel::OFF) return false;
            if (in_lvl < m_flushLevel) return false;

            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// Checks if we should flush.
        /// 
        /// @RETURNS {true} if there has been a log since last flush, else {false}.
        /// ----------------------------------------------------------------------------------------
        fn ShouldFlush() const noex -> bool
        {
            return m_alwaysFlush || m_hasWritten;
        }

    //// -------------------------------------------------------------------------------------------
    //// Implementation Functions
    //// -------------------------------------------------------------------------------------------

    protected:
        /// ----------------------------------------------------------------------------------------
        /// Write implementation.
        /// 
        /// @PARAM[IN] in_logMsg Log message object passed for logging.
        /// @PARAM[IN] in_formattedMsg Formatted message generated from {in_logMsg}.
        /// ----------------------------------------------------------------------------------------
        virtual fn m_Write(const LogMsg& in_logMsg, StrView in_formattedMsg) -> void abstract;

        /// ----------------------------------------------------------------------------------------
        /// Flush implementation.
        /// ----------------------------------------------------------------------------------------
        virtual fn m_Flush() -> void abstract;

    //// -------------------------------------------------------------------------------------------
    //// Variables
    //// -------------------------------------------------------------------------------------------

    protected:
        /// ----------------------------------------------------------------------------------------
        /// Log Level used to filter log messages.
        /// ----------------------------------------------------------------------------------------
        ELogLevel m_logLevel;

        /// ----------------------------------------------------------------------------------------
        /// Flush level used to check if to call flush after logging.
        /// ----------------------------------------------------------------------------------------
        ELogLevel m_flushLevel;

        /// ----------------------------------------------------------------------------------------
        /// Value used to check if there has been any write since last flush.
        /// ----------------------------------------------------------------------------------------
        bool m_hasWritten;

        /// ----------------------------------------------------------------------------------------
        /// If true always calls underlying flush m_Flush(), even if not necessary.
        /// This doesn't override CheckFlushLevel(ELogLevel in_lvl) check. It only affects 
        /// ShouldFlush().
        /// ----------------------------------------------------------------------------------------
        bool m_alwaysFlush;
    };
}