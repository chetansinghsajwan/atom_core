#pragma once
#include "Atom/Logging/LoggerRegistry.h"

namespace Atom::Logging
{
    inline LoggerPtr GET_ROOT_LOGGER()
    {
        return GET_REGISTRY().GetDefaultLogger();
    }

//// -----------------------------------------------------------------------------------------------
//// RUNTIME LOGGING
//// -----------------------------------------------------------------------------------------------

    /// ----------------------------------------------------------------------------------------
    /// Checks if RootLogger should log.
    /// 
    /// @PARAM[IN] lvl ELogLevel to check against.
    /// ----------------------------------------------------------------------------------------
    inline bool CHECK_LOG_LEVEL(ELogLevel lvl) noexcept
    {
        return GET_ROOT_LOGGER()->CheckLogLevel(lvl);
    }

    /// ----------------------------------------------------------------------------------------
    /// Calls Log on GET_ROOT_LOGGER().
    /// ----------------------------------------------------------------------------------------
    inline void LOG(ELogLevel lvl, const RLogString auto& msg, RLogArg auto&&... args)
    {
        GET_ROOT_LOGGER()->Log(lvl, msg, FORWARD(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// Calls LOG(ELogLevel::Trace, msg, FORWARD(args)...).
    /// --------------------------------------------------------------------------------------------
    inline void LOG_TRACE(const RLogString auto& msg, RLogArg auto&&... args)
    {
        LOG(ELogLevel::Trace, msg, FORWARD(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// Calls LOG(ELogLevel::Debug, msg, FORWARD(args)...).
    /// --------------------------------------------------------------------------------------------
    inline void LOG_DEBUG(const RLogString auto& msg, RLogArg auto&&... args)
    {
        LOG(ELogLevel::Debug, msg, FORWARD(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// Calls LOG(ELogLevel::Info, msg, FORWARD(args)...).
    /// --------------------------------------------------------------------------------------------
    inline void LOG_INFO(const RLogString auto& msg, RLogArg auto&&... args)
    {
        LOG(ELogLevel::Info, msg, FORWARD(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// Calls LOG(ELogLevel::Warn, msg, FORWARD(args)...).
    /// --------------------------------------------------------------------------------------------
    inline void LOG_WARN(const RLogString auto& msg, RLogArg auto&&... args)
    {
        LOG(ELogLevel::Warn, msg, FORWARD(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// Calls LOG(ELogLevel::Error, msg, FORWARD(args)...).
    /// --------------------------------------------------------------------------------------------
    inline void LOG_ERROR(const RLogString auto& msg, RLogArg auto&&... args)
    {
        LOG(ELogLevel::Error, msg, FORWARD(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// Calls LOG(ELogLevel::Fatal, msg, FORWARD(args)...).
    /// --------------------------------------------------------------------------------------------
    inline void LOG_FATAL(const RLogString auto& msg, RLogArg auto&&... args)
    {
        LOG(ELogLevel::Fatal, msg, FORWARD(args)...);
    }

    inline void FLUSH_LOGS()
    {
        GET_ROOT_LOGGER()->Flush();
    }

//// -----------------------------------------------------------------------------------------------
//// STATIC LOGGING
//// 
//// Logging is checked at compile time and hopefully removed at runtime.
//// -----------------------------------------------------------------------------------------------

    /// --------------------------------------------------------------------------------------------
    /// @TODO Add this as option.
    /// --------------------------------------------------------------------------------------------
    constexpr ELogLevel STATIC_LOG_LEVEL = ELogLevel::Debug;

    /// --------------------------------------------------------------------------------------------
    /// Checks at compile time if message of specified level {lvl} should be logged.
    /// 
    /// @TPARAM[IN] lvl ELogLevel of the msg to check for.
    /// --------------------------------------------------------------------------------------------
    template <ELogLevel lvl>
    constexpr bool STATIC_CHECK_LOG_LEVEL() noexcept
    {
        return lvl >= STATIC_LOG_LEVEL && lvl != ELogLevel::OFF;
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
    template <ELogLevel lvl>
    inline void STATIC_LOG(const RLogString auto& msg, RLogArg auto&&... args)
    {
        if constexpr (STATIC_CHECK_LOG_LEVEL<lvl>())
        {
            LOG(lvl, msg, FORWARD(args)...);
        }
    }

    /// --------------------------------------------------------------------------------------------
    /// Calls STATIC_LOG<ELogLevel::Trace>(msg, FORWARD(args)...).
    /// --------------------------------------------------------------------------------------------
    inline void STATIC_LOG_TRACE(const RLogString auto& msg, RLogArg auto&&... args)
    {
        STATIC_LOG<ELogLevel::Trace>(msg, FORWARD(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// Calls STATIC_LOG<ELogLevel::Debug>(msg, FORWARD(args)...).
    /// --------------------------------------------------------------------------------------------
    inline void STATIC_LOG_DEBUG(const RLogString auto& msg, RLogArg auto&&... args)
    {
        STATIC_LOG<ELogLevel::Debug>(msg, FORWARD(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// Calls STATIC_LOG<ELogLevel::Info>(msg, FORWARD(args)...).
    /// --------------------------------------------------------------------------------------------
    inline void STATIC_LOG_INFO(const RLogString auto& msg, RLogArg auto&&... args)
    {
        STATIC_LOG<ELogLevel::Info>(msg, FORWARD(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// Calls STATIC_LOG<ELogLevel::Warn>(msg, FORWARD(args)...).
    /// --------------------------------------------------------------------------------------------
    inline void STATIC_LOG_WARN(const RLogString auto& msg, RLogArg auto&&... args)
    {
        STATIC_LOG<ELogLevel::Warn>(msg, FORWARD(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// Calls STATIC_LOG<ELogLevel::Error>(msg, FORWARD(args)...).
    /// --------------------------------------------------------------------------------------------
    inline void STATIC_LOG_ERROR(const RLogString auto& msg, RLogArg auto&&... args)
    {
        STATIC_LOG<ELogLevel::Error>(msg, FORWARD(args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// Calls STATIC_LOG<ELogLevel::Fatal>(msg, FORWARD(args)...).
    /// --------------------------------------------------------------------------------------------
    inline void STATIC_LOG_FATAL(const RLogString auto& msg, RLogArg auto&&... args)
    {
        STATIC_LOG<ELogLevel::Fatal>(msg, FORWARD(args)...);
    }
}