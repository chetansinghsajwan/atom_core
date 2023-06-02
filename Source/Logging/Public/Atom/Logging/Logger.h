#pragma once
#include "LogMsg.h"
#include "Atom/Memory.h"

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
        /// Calls Log(ELogLevel::Trace, in_msg, FORWARD(in_args)...).
        /// ----------------------------------------------------------------------------------------
        template <RLogArg... TArgs>
        void LogTrace(LogString<TArgs...> in_msg, TArgs&&... in_args)
        {
            Log(ELogLevel::Trace, in_msg, FORWARD(in_args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Calls Log(ELogLevel::Debug, in_msg, FORWARD(in_args)...).
        /// ----------------------------------------------------------------------------------------
        template <RLogArg... TArgs>
        void LogDebug(LogString<TArgs...> in_msg, TArgs&&... in_args)
        {
            Log(ELogLevel::Debug, in_msg, FORWARD(in_args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Calls Log(ELogLevel::Info, in_msg, FORWARD(in_args)...).
        /// ----------------------------------------------------------------------------------------
        template <RLogArg... TArgs>
        void LogInfo(LogString<TArgs...> in_msg, TArgs&&... in_args)
        {
            Log(ELogLevel::Info, in_msg, FORWARD(in_args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Calls Log(ELogLevel::Warn, in_msg, FORWARD(in_args)...).
        /// ----------------------------------------------------------------------------------------
        template <RLogArg... TArgs>
        void LogWarn(LogString<TArgs...> in_msg, TArgs&&... in_args)
        {
            Log(ELogLevel::Warn, in_msg, FORWARD(in_args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Calls Log(ELogLevel::Error, in_msg, FORWARD(in_args)...).
        /// ----------------------------------------------------------------------------------------
        template <RLogArg... TArgs>
        void LogError(LogString<TArgs...> in_msg, TArgs&&... in_args)
        {
            Log(ELogLevel::Error, in_msg, FORWARD(in_args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Calls Log(ELogLevel::Fatal, in_msg, FORWARD(in_args)...).
        /// ----------------------------------------------------------------------------------------
        template <RLogArg... TArgs>
        void LogFatal(LogString<TArgs...> in_msg, TArgs&&... in_args)
        {
            Log(ELogLevel::Fatal, in_msg, FORWARD(in_args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Logs message if level is high enough to pass the filter level. Flushes if level passes 
        /// flush filter level.
        /// 
        /// @PARAM[IN] in_lvl ELogLevel to log at.
        /// @PARAM[IN] in_msg Log message containing format of the in_msg.
        /// @PARAM[IN] in_args... Arguments used with {in_msg} to construct the formatted message.
        /// ----------------------------------------------------------------------------------------
        template <RLogArg... TArgs>
        void Log(ELogLevel in_lvl, LogString<TArgs...> in_msg, TArgs&&... in_args)
        {
            if (CheckLogLevel(in_lvl))
            {
                String formattedMsg = StrFmter().Fmt(in_msg, FORWARD(in_args)...);
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
        virtual bool CheckLogLevel(ELogLevel in_lvl) const noexcept = 0;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ptr type used by this logging api to manage a Logger. It's recommended to use this 
    /// type to store logger instance. This keeps compatibility with this logging api.
    /// --------------------------------------------------------------------------------------------
    using LoggerPtr = SharedPtr<Logger>;

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <RDerivedFrom<Logger> TLogger>
    LoggerPtr MAKE_LOGGER(auto&&... in_args) noexcept
    {
        return MakeShared<TLogger>(FORWARD(in_args)...);
    }
}