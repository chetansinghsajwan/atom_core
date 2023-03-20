#pragma once
#include "Atom/Fmt.h"

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
        LogTargetBase() noexcept:
            m_logLevel(LogLevel::Debug), m_flushLevel(LogLevel::Info),
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
        virtual void Write(const LogMsg& in_logMsg) override final
        {
            if (CheckLogLevel(in_logMsg.lvl))
            {
                String result = Fmt::Format(TEXT("[{}] [{}] {}: {}\n"),
                    in_logMsg.time, in_logMsg.lvl, in_logMsg.loggerName, in_logMsg.msg);

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
        virtual void Flush() override final
        {
            if (ShouldFlush())
            {
                m_Flush();
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// Get the log level.
        /// ----------------------------------------------------------------------------------------
        LogLevel GetLogLevel() const noexcept
        {
            return m_logLevel;
        }

        /// ----------------------------------------------------------------------------------------
        /// Sets the log level.
        /// ----------------------------------------------------------------------------------------
        void SetLogLevel(LogLevel in_lvl) noexcept
        {
            m_logLevel = in_lvl;
        }

        /// ----------------------------------------------------------------------------------------
        /// Checks if we should log the message of specified level.
        /// ----------------------------------------------------------------------------------------
        bool CheckLogLevel(LogLevel in_lvl) const noexcept
        {
            if (in_lvl == LogLevel::OFF) return false;
            if (in_lvl < m_logLevel) return false;

            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// Gets the flush level.
        /// ----------------------------------------------------------------------------------------
        LogLevel GetFlushLevel() const noexcept
        {
            return m_flushLevel;
        }

        /// ----------------------------------------------------------------------------------------
        /// Sets the flush level.
        /// ----------------------------------------------------------------------------------------
        void SetFlushLevel(LogLevel in_lvl) noexcept
        {
            m_flushLevel = in_lvl;
        }

        /// ----------------------------------------------------------------------------------------
        /// Checks if should flush after logging the message of specified level.
        /// It also asks ShouldFlush().
        /// ----------------------------------------------------------------------------------------
        bool CheckFlushLevel(LogLevel in_lvl) const noexcept
        {
            if (!m_hasWritten) return false;
            if (in_lvl == LogLevel::OFF) return false;
            if (in_lvl < m_flushLevel) return false;

            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// Checks if we should flush.
        /// 
        /// @RETURNS {true} if there has been a log since last flush, else {false}.
        /// ----------------------------------------------------------------------------------------
        bool ShouldFlush() const noexcept
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
        virtual void m_Write(const LogMsg& in_logMsg, StringView in_formattedMsg) = 0;

        /// ----------------------------------------------------------------------------------------
        /// Flush implementation.
        /// ----------------------------------------------------------------------------------------
        virtual void m_Flush() = 0;

    //// -------------------------------------------------------------------------------------------
    //// Variables
    //// -------------------------------------------------------------------------------------------

    protected:
        /// ----------------------------------------------------------------------------------------
        /// Log Level used to filter log messages.
        /// ----------------------------------------------------------------------------------------
        LogLevel m_logLevel;

        /// ----------------------------------------------------------------------------------------
        /// Flush level used to check if to call flush after logging.
        /// ----------------------------------------------------------------------------------------
        LogLevel m_flushLevel;

        /// ----------------------------------------------------------------------------------------
        /// Value used to check if there has been any write since last flush.
        /// ----------------------------------------------------------------------------------------
        bool m_hasWritten;

        /// ----------------------------------------------------------------------------------------
        /// If true always calls underlying flush m_Flush(), even if not necessary.
        /// This doesn't override CheckFlushLevel(LogLevel in_lvl) check. It only affects 
        /// ShouldFlush().
        /// ----------------------------------------------------------------------------------------
        bool m_alwaysFlush;
    };
}