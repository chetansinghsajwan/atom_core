#pragma once
#include "fmt/format.h"
#include "fmt/chrono.h"
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
            _logLevel(LogLevel::Debug), _flushLevel(LogLevel::Info),
            _hasWritten(false), _alwaysFlush(false) { }

    //// -------------------------------------------------------------------------------------------
    //// API Functions
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// Filters and Formats the LogMsg and passes it to write.
        /// 
        /// @EXCEPTION_SAFETY Strong.
        /// 
        /// @SEE _Write().
        /// ----------------------------------------------------------------------------------------
        virtual void Write(const LogMsg& logMsg) override final
        {
            if (CheckLogLevel(logMsg.lvl))
            {
                String result = fmt::format("[{}] [{}] {}: {}\n",
                    logMsg.time, logMsg.lvl, logMsg.loggerName, logMsg.msg);

                _hasWritten = true;
                _Write(logMsg, result);

                if (CheckFlushLevel(logMsg.lvl))
                {
                    _Flush();
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
                _Flush();
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// Get the log level.
        /// ----------------------------------------------------------------------------------------
        LogLevel GetLogLevel() const noexcept
        {
            return _logLevel;
        }

        /// ----------------------------------------------------------------------------------------
        /// Sets the log level.
        /// ----------------------------------------------------------------------------------------
        void SetLogLevel(LogLevel lvl) noexcept
        {
            _logLevel = lvl;
        }

        /// ----------------------------------------------------------------------------------------
        /// Checks if we should log the message of specified level.
        /// ----------------------------------------------------------------------------------------
        bool CheckLogLevel(LogLevel lvl) const noexcept
        {
            if (lvl == LogLevel::OFF) return false;
            if (lvl < _logLevel) return false;

            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// Gets the flush level.
        /// ----------------------------------------------------------------------------------------
        LogLevel GetFlushLevel() const noexcept
        {
            return _flushLevel;
        }

        /// ----------------------------------------------------------------------------------------
        /// Sets the flush level.
        /// ----------------------------------------------------------------------------------------
        void SetFlushLevel(LogLevel lvl) noexcept
        {
            _flushLevel = lvl;
        }

        /// ----------------------------------------------------------------------------------------
        /// Checks if should flush after logging the message of specified level.
        /// It also asks ShouldFlush().
        /// ----------------------------------------------------------------------------------------
        bool CheckFlushLevel(LogLevel lvl) const noexcept
        {
            if (!_hasWritten) return false;
            if (lvl == LogLevel::OFF) return false;
            if (lvl < _flushLevel) return false;

            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// Checks if we should flush.
        /// 
        /// @RETURNS {true} if there has been a log since last flush, else {false}.
        /// ----------------------------------------------------------------------------------------
        bool ShouldFlush() const noexcept
        {
            return _alwaysFlush || _hasWritten;
        }

    //// -------------------------------------------------------------------------------------------
    //// Implementation Functions
    //// -------------------------------------------------------------------------------------------

    protected:
        /// ----------------------------------------------------------------------------------------
        /// Write implementation.
        /// 
        /// @PARAM[IN] logMsg Log message object passed for logging.
        /// @PARAM[IN] formattedMsg Formatted message generated from {logMsg}.
        /// ----------------------------------------------------------------------------------------
        virtual void _Write(const LogMsg& logMsg, StringView formattedMsg) = 0;

        /// ----------------------------------------------------------------------------------------
        /// Flush implementation.
        /// ----------------------------------------------------------------------------------------
        virtual void _Flush() = 0;

    //// -------------------------------------------------------------------------------------------
    //// Variables
    //// -------------------------------------------------------------------------------------------

    protected:
        /// ----------------------------------------------------------------------------------------
        /// Log Level used to filter log messages.
        /// ----------------------------------------------------------------------------------------
        LogLevel _logLevel;

        /// ----------------------------------------------------------------------------------------
        /// Flush level used to check if to call flush after logging.
        /// ----------------------------------------------------------------------------------------
        LogLevel _flushLevel;

        /// ----------------------------------------------------------------------------------------
        /// Value used to check if there has been any write since last flush.
        /// ----------------------------------------------------------------------------------------
        bool _hasWritten;

        /// ----------------------------------------------------------------------------------------
        /// If true always calls underlying flush _Flush(), even if not necessary.
        /// This doesn't override CheckFlushLevel(LogLevel lvl) check. It only affects 
        /// ShouldFlush().
        /// ----------------------------------------------------------------------------------------
        bool _alwaysFlush;
    };
}