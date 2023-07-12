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
        virtual fn Name() const noex -> StrView abstract;

        /// ----------------------------------------------------------------------------------------
        /// Calls Log(ELogLevel::Trace, in_msg, fwd(in_args)...).
        /// ----------------------------------------------------------------------------------------
        template <RLogArg... TArgs>
        fn LogTrace(LogStr<TArgs...> in_msg, TArgs&&... in_args)
        {
            Log(ELogLevel::Trace, in_msg, fwd(in_args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Calls Log(ELogLevel::Debug, in_msg, fwd(in_args)...).
        /// ----------------------------------------------------------------------------------------
        template <RLogArg... TArgs>
        fn LogDebug(LogStr<TArgs...> in_msg, TArgs&&... in_args)
        {
            Log(ELogLevel::Debug, in_msg, fwd(in_args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Calls Log(ELogLevel::Info, in_msg, fwd(in_args)...).
        /// ----------------------------------------------------------------------------------------
        template <RLogArg... TArgs>
        fn LogInfo(LogStr<TArgs...> in_msg, TArgs&&... in_args)
        {
            Log(ELogLevel::Info, in_msg, fwd(in_args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Calls Log(ELogLevel::Warn, in_msg, fwd(in_args)...).
        /// ----------------------------------------------------------------------------------------
        template <RLogArg... TArgs>
        fn LogWarn(LogStr<TArgs...> in_msg, TArgs&&... in_args)
        {
            Log(ELogLevel::Warn, in_msg, fwd(in_args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Calls Log(ELogLevel::Error, in_msg, fwd(in_args)...).
        /// ----------------------------------------------------------------------------------------
        template <RLogArg... TArgs>
        fn LogError(LogStr<TArgs...> in_msg, TArgs&&... in_args)
        {
            Log(ELogLevel::Error, in_msg, fwd(in_args)...);
        }

        /// ----------------------------------------------------------------------------------------
        /// Calls Log(ELogLevel::Fatal, in_msg, fwd(in_args)...).
        /// ----------------------------------------------------------------------------------------
        template <RLogArg... TArgs>
        fn LogFatal(LogStr<TArgs...> in_msg, TArgs&&... in_args)
        {
            Log(ELogLevel::Fatal, in_msg, fwd(in_args)...);
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
        fn Log(ELogLevel in_lvl, LogStr<TArgs...> in_msg, TArgs&&... in_args)
        {
            if (CheckLogLevel(in_lvl))
            {
                Str formattedMsg = StrFmter().Fmt(in_msg, fwd(in_args)...);
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
        virtual fn Log(LogMsg& in_logMsg) -> void abstract;

        /// ----------------------------------------------------------------------------------------
        /// Pure virtual function.
        /// 
        /// Flushes the logs of this logger.
        /// ----------------------------------------------------------------------------------------
        virtual fn Flush() -> void abstract;

        /// ----------------------------------------------------------------------------------------
        /// Pure virtual function.
        /// 
        /// Check if the message should be passed for logging.
        /// ----------------------------------------------------------------------------------------
        virtual fn CheckLogLevel(ELogLevel in_lvl) const noex -> bool abstract;
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
    LoggerPtr MAKE_LOGGER(auto&&... in_args) noex
    {
        return MakeShared<TLogger>(fwd(in_args)...);
    }
}