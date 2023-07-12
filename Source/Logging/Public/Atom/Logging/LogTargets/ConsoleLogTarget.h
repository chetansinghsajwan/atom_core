#pragma once
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
        /// ErrorLogLevel is set to ELogLevel::Error.
        /// ----------------------------------------------------------------------------------------
        ConsoleLogTarget() noex:
            _stdout(stdout), _stderr(stderr),
            _errLogLevel(ELogLevel::Error) { }

    public:
        /// ----------------------------------------------------------------------------------------
        /// Retuns the ELogLevel specified for error logging.
        /// ----------------------------------------------------------------------------------------
        ELogLevel GetErrLogLevel() const noex
        {
            return _errLogLevel;
        }

        /// ----------------------------------------------------------------------------------------
        /// Sets the ELogLevel specified for error logging.
        /// 
        /// Logs of same or above level are written to stderr.
        /// ----------------------------------------------------------------------------------------
        void SetErrLogLevel(ELogLevel lvl) noex
        {
            _errLogLevel = lvl;
        }

        /// ----------------------------------------------------------------------------------------
        /// Checks if ELogLevel should be written to stderr.
        /// 
        /// @RETURNS {true} if {lvl >= GetErrLogLevel()}, else {false}.
        /// ----------------------------------------------------------------------------------------
        bool ShouldLogAsError(ELogLevel lvl) const noex
        {
            return lvl >= _errLogLevel;
        }

    protected:
        /// ----------------------------------------------------------------------------------------
        /// Writes the formatted message to stdout or stderr based on log level.
        /// 
        /// If {ShouldLogAsError(logMsg.lvl) == true}, writes the message to stderr else to stdout.
        /// ----------------------------------------------------------------------------------------
        virtual void m_Write(const LogMsg& logMsg, StrView formattedMsg) ofinal
        {
            FILE* file = _stdout;

            if (ShouldLogAsError(logMsg.lvl))
            {
                file = _stderr;
            }

            ::fwrite(formattedMsg.Data(), sizeof(char), formattedMsg.Count(), file);
        }

        /// ----------------------------------------------------------------------------------------
        /// Flushes both stdout and stderr.
        /// ----------------------------------------------------------------------------------------
        virtual void m_Flush() ofinal
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
        ELogLevel _errLogLevel;
    };
}