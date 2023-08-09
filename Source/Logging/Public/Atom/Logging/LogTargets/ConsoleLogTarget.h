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
        ctor ConsoleLogTarget():
            _stdout(stdout), _stderr(stderr),
            _errLogLevel(ELogLevel::Error) { }

    public:
        /// ----------------------------------------------------------------------------------------
        /// Retuns the ELogLevel specified for error logging.
        /// ----------------------------------------------------------------------------------------
        fn GetErrLogLevel() const -> ELogLevel
        {
            return _errLogLevel;
        }

        /// ----------------------------------------------------------------------------------------
        /// Sets the ELogLevel specified for error logging.
        /// 
        /// Logs of same or above level are written to stderr.
        /// ----------------------------------------------------------------------------------------
        fn SetErrLogLevel(ELogLevel lvl)
        {
            _errLogLevel = lvl;
        }

        /// ----------------------------------------------------------------------------------------
        /// Checks if ELogLevel should be written to stderr.
        /// 
        /// @RETURNS {true} if {lvl >= GetErrLogLevel()}, else {false}.
        /// ----------------------------------------------------------------------------------------
        fn ShouldLogAsError(ELogLevel lvl) const -> bool
        {
            return lvl >= _errLogLevel;
        }

    protected:
        /// ----------------------------------------------------------------------------------------
        /// Writes the formatted message to stdout or stderr based on log level.
        /// 
        /// If {ShouldLogAsError(logMsg.lvl) == true}, writes the message to stderr else to stdout.
        /// ----------------------------------------------------------------------------------------
        virtual fn _Write(const LogMsg& logMsg, StrView formattedMsg) -> void ofinal
        {
            FILE* file = _stdout;

            if (ShouldLogAsError(logMsg.lvl))
            {
                file = _stderr;
            }

            ::fwrite(formattedMsg.data(), sizeof(char), formattedMsg.count(), file);
        }

        /// ----------------------------------------------------------------------------------------
        /// Flushes both stdout and stderr.
        /// ----------------------------------------------------------------------------------------
        virtual fn _Flush() -> void ofinal
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