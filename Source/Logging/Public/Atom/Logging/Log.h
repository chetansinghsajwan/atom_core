#pragma once
#include "Atom/Logging/LoggerRegistry.h"

namespace Atom::Logging
{
    LoggerPtr GET_ROOT_LOGGER()
    {
        return GET_REGISTRY().GetDefaultLogger();
    }

//// -----------------------------------------------------------------------------------------------
//// RUNTIME LOGGING
//// -----------------------------------------------------------------------------------------------

    /// ----------------------------------------------------------------------------------------
    /// Checks if RootLogger should log.
    /// 
    /// @PARAM[IN] lvl LogLevel to check against.
    /// ----------------------------------------------------------------------------------------
    inline bool CHECK_LOG_LEVEL(LogLevel lvl) noexcept
    {
        return GET_ROOT_LOGGER()->CheckLogLevel(lvl);
    }

    /// ----------------------------------------------------------------------------------------
    /// Calls Log on GET_ROOT_LOGGER().
    /// ----------------------------------------------------------------------------------------
    inline void LOG(LogLevel lvl, const LogStringType auto& msg, LogArgType auto&&... args)
    {
        GET_ROOT_LOGGER()->Log(lvl, msg, FWD(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// Calls LOG(LogLevel::Trace, msg, FWD(args)...).
    /// --------------------------------------------------------------------------------------------
    inline void LOG_TRACE(const LogStringType auto& msg, LogArgType auto&&... args)
    {
        LOG(LogLevel::Trace, msg, FWD(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// Calls LOG(LogLevel::Debug, msg, FWD(args)...).
    /// --------------------------------------------------------------------------------------------
    inline void LOG_DEBUG(const LogStringType auto& msg, LogArgType auto&&... args)
    {
        LOG(LogLevel::Debug, msg, FWD(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// Calls LOG(LogLevel::Info, msg, FWD(args)...).
    /// --------------------------------------------------------------------------------------------
    inline void LOG_INFO(const LogStringType auto& msg, LogArgType auto&&... args)
    {
        LOG(LogLevel::Info, msg, FWD(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// Calls LOG(LogLevel::Warn, msg, FWD(args)...).
    /// --------------------------------------------------------------------------------------------
    inline void LOG_WARN(const LogStringType auto& msg, LogArgType auto&&... args)
    {
        LOG(LogLevel::Warn, msg, FWD(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// Calls LOG(LogLevel::Error, msg, FWD(args)...).
    /// --------------------------------------------------------------------------------------------
    inline void LOG_ERROR(const LogStringType auto& msg, LogArgType auto&&... args)
    {
        LOG(LogLevel::Error, msg, FWD(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// Calls LOG(LogLevel::Fatal, msg, FWD(args)...).
    /// --------------------------------------------------------------------------------------------
    inline void LOG_FATAL(const LogStringType auto& msg, LogArgType auto&&... args)
    {
        LOG(LogLevel::Fatal, msg, FWD(args)...);
    }

    void FLUSH_LOGS()
    {
        GET_ROOT_LOGGER()->Flush();
    }

//// -----------------------------------------------------------------------------------------------
//// STATIC LOGGING
//// 
//// Logging is checked at compile time and hopefully removed at runtime.
//// -----------------------------------------------------------------------------------------------

    /// --------------------------------------------------------------------------------------------
    /// Checks at compile time if message of specified level {lvl} should be logged.
    /// 
    /// @TPARAM[IN] lvl LogLevel of the msg to check for.
    /// --------------------------------------------------------------------------------------------
    template <LogLevel lvl>
    constexpr bool STATIC_CHECK_LOG_LEVEL() noexcept
    {
        /// @TODO Add this as option.
        static constexpr LogLevel staticLevel = LogLevel::Debug;

        return lvl >= staticLevel && lvl != LogLevel::OFF;
    }

    /// --------------------------------------------------------------------------------------------
    /// Checks at compile time if message should be logged. If not it's just an empty function
    /// and hopefully compiler will optimize it away, however it's yet to check.
    /// 
    /// This is done to avoid the use of macros. But if this approach fails we may fallback to 
    /// macros.
    /// 
    /// @TODO Check the overhead of this function when it should not log anything.
    /// --------------------------------------------------------------------------------------------
    template <LogLevel lvl>
    inline void STATIC_LOG(const LogStringType auto& msg, LogArgType auto&&... args)
    {
        if constexpr (STATIC_CHECK_LOG_LEVEL<lvl>())
        {
            LOG(lvl, msg, FWD(args)...);
        }
    }

    /// --------------------------------------------------------------------------------------------
    /// Calls STATIC_LOG<LogLevel::Trace>(msg, FWD(args)...).
    /// --------------------------------------------------------------------------------------------
    inline void STATIC_LOG_TRACE(const LogStringType auto& msg, LogArgType auto&&... args)
    {
        STATIC_LOG<LogLevel::Trace>(msg, FWD(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// Calls STATIC_LOG<LogLevel::Debug>(msg, FWD(args)...).
    /// --------------------------------------------------------------------------------------------
    inline void STATIC_LOG_DEBUG(const LogStringType auto& msg, LogArgType auto&&... args)
    {
        STATIC_LOG<LogLevel::Debug>(msg, FWD(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// Calls STATIC_LOG<LogLevel::Info>(msg, FWD(args)...).
    /// --------------------------------------------------------------------------------------------
    inline void STATIC_LOG_INFO(const LogStringType auto& msg, LogArgType auto&&... args)
    {
        STATIC_LOG<LogLevel::Info>(msg, FWD(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// Calls STATIC_LOG<LogLevel::Warn>(msg, FWD(args)...).
    /// --------------------------------------------------------------------------------------------
    inline void STATIC_LOG_WARN(const LogStringType auto& msg, LogArgType auto&&... args)
    {
        STATIC_LOG<LogLevel::Warn>(msg, FWD(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// Calls STATIC_LOG<LogLevel::Error>(msg, FWD(args)...).
    /// --------------------------------------------------------------------------------------------
    inline void STATIC_LOG_ERROR(const LogStringType auto& msg, LogArgType auto&&... args)
    {
        STATIC_LOG<LogLevel::Error>(msg, FWD(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// Calls STATIC_LOG<LogLevel::Fatal>(msg, FWD(args)...).
    /// --------------------------------------------------------------------------------------------
    inline void STATIC_LOG_FATAL(const LogStringType auto& msg, LogArgType auto&&... args)
    {
        STATIC_LOG<LogLevel::Fatal>(msg, FWD(args)...);
    }
}