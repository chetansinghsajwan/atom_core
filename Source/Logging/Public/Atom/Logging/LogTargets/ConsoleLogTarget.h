#pragma once
#include <stddef.h>
#include "Atom/Logging/LogTargets/LogTargetBase.h"

namespace Atom::Logging
{
    /// --------------------------------------------------------------------------------------------
    /// ConsoleLogTarget logs to console using stdout and stderr.
    /// --------------------------------------------------------------------------------------------
    class ConsoleLogTarget : public Internal::LogTargetBase
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// Default constructs the ConsoleLogTarget.
        /// 
        /// ErrorLogLevel is set to LogLevel::Error.
        /// ----------------------------------------------------------------------------------------
        ConsoleLogTarget() noexcept:
            _stdout(stdout), _stderr(stderr),
            _errLogLevel(LogLevel::Error) { }

    public:
        /// ----------------------------------------------------------------------------------------
        /// Retuns the LogLevel specified for error logging.
        /// ----------------------------------------------------------------------------------------
        LogLevel GetErrLogLevel() const noexcept
        {
            return _errLogLevel;
        }

        /// ----------------------------------------------------------------------------------------
        /// Sets the LogLevel specified for error logging.
        /// 
        /// Logs of same or above level are written to stderr.
        /// ----------------------------------------------------------------------------------------
        void SetErrLogLevel(LogLevel lvl) noexcept
        {
            _errLogLevel = lvl;
        }

        /// ----------------------------------------------------------------------------------------
        /// Checks if LogLevel should be written to stderr.
        /// 
        /// @RETURNS {true} if {lvl >= GetErrLogLevel()}, else {false}.
        /// ----------------------------------------------------------------------------------------
        bool ShouldLogAsError(LogLevel lvl) const noexcept
        {
            return lvl >= _errLogLevel;
        }

    protected:
        /// ----------------------------------------------------------------------------------------
        /// Writes the formatted message to stdout or stderr based on log level.
        /// 
        /// If {ShouldLogAsError(logMsg.lvl) == true}, writes the message to stderr else to stdout.
        /// ----------------------------------------------------------------------------------------
        virtual void m_Write(const LogMsg& logMsg, StringView formattedMsg) override final
        {
            FILE* file = _stdout;

            if (ShouldLogAsError(logMsg.lvl))
            {
                file = _stderr;
            }

            ::fwrite(formattedMsg.data(), sizeof(char), formattedMsg.size(), file);
        }

        /// ----------------------------------------------------------------------------------------
        /// Flushes both stdout and stderr.
        /// ----------------------------------------------------------------------------------------
        virtual void m_Flush() override final
        {
            fflush(_stdout);
            fflush(_stderr);
        }

    protected:

        /// ----------------------------------------------------------------------------------------
        /// FILE* to stdout.
        /// ----------------------------------------------------------------------------------------
        FILE* _stdout;

        /// ----------------------------------------------------------------------------------------
        /// FILE* to stderr.
        /// ----------------------------------------------------------------------------------------
        FILE* _stderr;

        /// ----------------------------------------------------------------------------------------
        /// ErrorLogLevel used to check if the message should be logged to stderr or stdout.
        /// ----------------------------------------------------------------------------------------
        LogLevel _errLogLevel;
    };
}