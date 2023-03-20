#pragma once
#include "Atom/Memory.h"
#include "Atom/Fmt.h"

#include "Atom/Logging/LogMsg.h"

namespace Atom::Logging
{
    class Logger
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// Get the name of the logger.
        /// ----------------------------------------------------------------------------------------
        virtual StringView Name() const noexcept = 0;

        /// ----------------------------------------------------------------------------------------
        /// Calls Log(LogLevel::Trace, in_msg, FORWARD(in_args)...).
        /// ----------------------------------------------------------------------------------------
        void LogTrace(const RLogString auto& in_msg, RLogArg auto&&... in_args)
        {
            Log(LogLevel::Trace, in_msg, FORWARD(in_args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Calls Log(LogLevel::Debug, in_msg, FORWARD(in_args)...).
        /// ----------------------------------------------------------------------------------------
        void LogDebug(const RLogString auto& in_msg, RLogArg auto&&... in_args)
        {
            Log(LogLevel::Debug, in_msg, FORWARD(in_args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Calls Log(LogLevel::Info, in_msg, FORWARD(in_args)...).
        /// ----------------------------------------------------------------------------------------
        void LogInfo(const RLogString auto& in_msg, RLogArg auto&&... in_args)
        {
            Log(LogLevel::Info, in_msg, FORWARD(in_args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Calls Log(LogLevel::Warn, in_msg, FORWARD(in_args)...).
        /// ----------------------------------------------------------------------------------------
        void LogWarn(const RLogString auto& in_msg, RLogArg auto&&... in_args)
        {
            Log(LogLevel::Warn, in_msg, FORWARD(in_args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Calls Log(LogLevel::Error, in_msg, FORWARD(in_args)...).
        /// ----------------------------------------------------------------------------------------
        void LogError(const RLogString auto& in_msg, RLogArg auto&&... in_args)
        {
            Log(LogLevel::Error, in_msg, FORWARD(in_args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Calls Log(LogLevel::Fatal, in_msg, FORWARD(in_args)...).
        /// ----------------------------------------------------------------------------------------
        void LogFatal(const RLogString auto& in_msg, RLogArg auto&&... in_args)
        {
            Log(LogLevel::Fatal, in_msg, FORWARD(in_args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Logs message if level is high enough to pass the filter level. Flushes if level passes 
        /// flush filter level.
        /// 
        /// @PARAM[IN] in_lvl LogLevel to log at.
        /// @PARAM[IN] in_msg Log message containing format of the in_msg.
        /// @PARAM[IN] in_args... Arguments used with {in_msg} to construct the formatted message.
        /// ----------------------------------------------------------------------------------------
        void Log(LogLevel in_lvl, const RLogString auto& in_msg, RLogArg auto&&... in_args)
        {
            if (CheckLogLevel(in_lvl))
            {
                String formattedMsg = Fmt::Format(Fmt::Runtime(in_msg), FORWARD(in_args)...);
                LogMsg logMsg
                {
                    .msg = formattedMsg,
                    .loggerName = Name(),
                    .lvl = in_lvl,
                    .time = Time::Now(),
                };

                Log(logMsg);
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// Pure virtual function.
        /// 
        /// Logs the LogMsg object.
        /// ----------------------------------------------------------------------------------------
        virtual void Log(LogMsg& in_logMsg) = 0;

        /// ----------------------------------------------------------------------------------------
        /// Pure virtual function.
        /// 
        /// Flushes the logs of this logger.
        /// ----------------------------------------------------------------------------------------
        virtual void Flush() = 0;

        /// ----------------------------------------------------------------------------------------
        /// Pure virtual function.
        /// 
        /// Check if the message should be passed for logging.
        /// ----------------------------------------------------------------------------------------
        virtual bool CheckLogLevel(LogLevel in_lvl) const noexcept = 0;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ptr type used by this logging api to manage a Logger. It's recommended to use this 
    /// type to store logger instance. This keeps compatibility with this logging api.
    /// --------------------------------------------------------------------------------------------
    using LoggerPtr = SharedPtr<Logger>;

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TLogger>
        requires TTI::IsBaseOf<Logger, TLogger>
    LoggerPtr MAKE_LOGGER(auto&&... in_args) noexcept
    {
        return MakeShared<TLogger>(FORWARD(in_args)...);
    }
}