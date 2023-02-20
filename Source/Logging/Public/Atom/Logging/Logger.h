#pragma once
#include "fmt/format.h"
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
        /// Calls Log(LogLevel::Trace, msg, FWD(args)...).
        /// ----------------------------------------------------------------------------------------
        void LogTrace(const LogStringType auto& msg, LogArgType auto&&... args)
        {
            Log(LogLevel::Trace, msg, FWD(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Calls Log(LogLevel::Debug, msg, FWD(args)...).
        /// ----------------------------------------------------------------------------------------
        void LogDebug(const LogStringType auto& msg, LogArgType auto&&... args)
        {
            Log(LogLevel::Debug, msg, FWD(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Calls Log(LogLevel::Info, msg, FWD(args)...).
        /// ----------------------------------------------------------------------------------------
        void LogInfo(const LogStringType auto& msg, LogArgType auto&&... args)
        {
            Log(LogLevel::Info, msg, FWD(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Calls Log(LogLevel::Warn, msg, FWD(args)...).
        /// ----------------------------------------------------------------------------------------
        void LogWarn(const LogStringType auto& msg, LogArgType auto&&... args)
        {
            Log(LogLevel::Warn, msg, FWD(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Calls Log(LogLevel::Error, msg, FWD(args)...).
        /// ----------------------------------------------------------------------------------------
        void LogError(const LogStringType auto& msg, LogArgType auto&&... args)
        {
            Log(LogLevel::Error, msg, FWD(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Calls Log(LogLevel::Fatal, msg, FWD(args)...).
        /// ----------------------------------------------------------------------------------------
        void LogFatal(const LogStringType auto& msg, LogArgType auto&&... args)
        {
            Log(LogLevel::Fatal, msg, FWD(args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Logs message if level is high enough to pass the filter level. Flushes if level passes 
        /// flush filter level.
        /// 
        /// @PARAM[IN] lvl LogLevel to log at.
        /// @PARAM[IN] msg Log message containing format of the msg.
        /// @PARAM[IN] args... Arguments used with {msg} to construct the formatted message.
        /// ----------------------------------------------------------------------------------------
        void Log(LogLevel lvl, const LogStringType auto& msg, LogArgType auto&&... args)
        {
            if (ShouldLog(lvl))
            {
                String formattedMsg = fmt::format(msg, FWD(args)...);
                LogMsg logMsg
                {
                    .msg = formattedMsg,
                    .loggerName = Name(),
                    .lvl = lvl,
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
        virtual void Log(LogMsg& logMsg) = 0;

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
        virtual bool ShouldLog(LogLevel lvl) const noexcept = 0;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ptr type used by this logging api to manage a Logger. It's recommended to use this 
    /// type to store logger instance. This keeps compatibility with thes logging api.
    /// --------------------------------------------------------------------------------------------
    using LoggerPtr = SharedPtr<Logger>;

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename LoggerType>
    requires std::is_base_of_v<Logger, LoggerType>
    LoggerPtr MAKE_LOGGER(auto&&... args) noexcept
    {
        return MakeShared<LoggerType>(FWD(args)...);
    }
}