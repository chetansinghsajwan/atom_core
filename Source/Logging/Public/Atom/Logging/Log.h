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
    /// @PARAM[IN] in_lvl ELogLevel to check against.
    /// ----------------------------------------------------------------------------------------
    inline bool CHECK_LOG_LEVEL(ELogLevel in_lvl) noexcept
    {
        return GET_ROOT_LOGGER()->CheckLogLevel(in_lvl);
    }

    /// ----------------------------------------------------------------------------------------
    /// Calls Log on GET_ROOT_LOGGER().
    /// ----------------------------------------------------------------------------------------
    template <RLogArg... TArgs>
    inline void LOG(ELogLevel in_lvl, LogString<TArgs...> in_msg, TArgs&&... in_args)
    {
        GET_ROOT_LOGGER()->Log(in_lvl, in_msg, FORWARD(in_args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// Calls LOG(ELogLevel::Trace, in_msg, FORWARD(in_args)...).
    /// --------------------------------------------------------------------------------------------
    template <RLogArg... TArgs>
    inline void LOG_TRACE(LogString<TArgs...> in_msg, TArgs&&... in_args)
    {
        LOG(ELogLevel::Trace, in_msg, FORWARD(in_args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// Calls LOG(ELogLevel::Debug, in_msg, FORWARD(in_args)...).
    /// --------------------------------------------------------------------------------------------
    template <RLogArg... TArgs>
    inline void LOG_DEBUG(LogString<TArgs...> in_msg, TArgs&&... in_args)
    {
        LOG(ELogLevel::Debug, in_msg, FORWARD(in_args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// Calls LOG(ELogLevel::Info, in_msg, FORWARD(in_args)...).
    /// --------------------------------------------------------------------------------------------
    template <RLogArg... TArgs>
    inline void LOG_INFO(LogString<TArgs...> in_msg, TArgs&&... in_args)
    {
        LOG(ELogLevel::Info, in_msg, FORWARD(in_args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// Calls LOG(ELogLevel::Warn, in_msg, FORWARD(in_args)...).
    /// --------------------------------------------------------------------------------------------
    template <RLogArg... TArgs>
    inline void LOG_WARN(LogString<TArgs...> in_msg, TArgs&&... in_args)
    {
        LOG(ELogLevel::Warn, in_msg, FORWARD(in_args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// Calls LOG(ELogLevel::Error, in_msg, FORWARD(in_args)...).
    /// --------------------------------------------------------------------------------------------
    template <RLogArg... TArgs>
    inline void LOG_ERROR(LogString<TArgs...> in_msg, TArgs&&... in_args)
    {
        LOG(ELogLevel::Error, in_msg, FORWARD(in_args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// Calls LOG(ELogLevel::Fatal, in_msg, FORWARD(in_args)...).
    /// --------------------------------------------------------------------------------------------
    template <RLogArg... TArgs>
    inline void LOG_FATAL(LogString<TArgs...> in_msg, TArgs&&... in_args)
    {
        LOG(ELogLevel::Fatal, in_msg, FORWARD(in_args)...);
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
    /// Checks at compile time if message of specified level {in_lvl} should be logged.
    /// 
    /// @TPARAM[IN] in_lvl ELogLevel of the in_msg to check for.
    /// --------------------------------------------------------------------------------------------
    template <ELogLevel in_lvl>
    constexpr bool STATIC_CHECK_LOG_LEVEL() noexcept
    {
        return in_lvl >= STATIC_LOG_LEVEL && in_lvl != ELogLevel::OFF;
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
    template <ELogLevel in_lvl, RLogArg... TArgs>
    inline void STATIC_LOG(LogString<TArgs...> in_msg, TArgs&&... in_args)
    {
        if constexpr (STATIC_CHECK_LOG_LEVEL<in_lvl>())
        {
            LOG(in_lvl, in_msg, FORWARD(in_args)...);
        }
    }

    /// --------------------------------------------------------------------------------------------
    /// Calls STATIC_LOG<ELogLevel::Trace>(in_msg, FORWARD(in_args)...).
    /// --------------------------------------------------------------------------------------------
    template <RLogArg... TArgs>
    inline void STATIC_LOG_TRACE(LogString<TArgs...> in_msg, TArgs&&... in_args)
    {
        STATIC_LOG<ELogLevel::Trace>(in_msg, FORWARD(in_args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// Calls STATIC_LOG<ELogLevel::Debug>(in_msg, FORWARD(in_args)...).
    /// --------------------------------------------------------------------------------------------
    template <RLogArg... TArgs>
    inline void STATIC_LOG_DEBUG(LogString<TArgs...> in_msg, TArgs&&... in_args)
    {
        STATIC_LOG<ELogLevel::Debug>(in_msg, FORWARD(in_args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// Calls STATIC_LOG<ELogLevel::Info>(in_msg, FORWARD(in_args)...).
    /// --------------------------------------------------------------------------------------------
    template <RLogArg... TArgs>
    inline void STATIC_LOG_INFO(LogString<TArgs...> in_msg, TArgs&&... in_args)
    {
        STATIC_LOG<ELogLevel::Info>(in_msg, FORWARD(in_args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// Calls STATIC_LOG<ELogLevel::Warn>(in_msg, FORWARD(in_args)...).
    /// --------------------------------------------------------------------------------------------
    template <RLogArg... TArgs>
    inline void STATIC_LOG_WARN(LogString<TArgs...> in_msg, TArgs&&... in_args)
    {
        STATIC_LOG<ELogLevel::Warn>(in_msg, FORWARD(in_args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// Calls STATIC_LOG<ELogLevel::Error>(in_msg, FORWARD(in_args)...).
    /// --------------------------------------------------------------------------------------------
    template <RLogArg... TArgs>
    inline void STATIC_LOG_ERROR(LogString<TArgs...> in_msg, TArgs&&... in_args)
    {
        STATIC_LOG<ELogLevel::Error>(in_msg, FORWARD(in_args)...);
    }

    /// --------------------------------------------------------------------------------------------
    /// Calls STATIC_LOG<ELogLevel::Fatal>(in_msg, FORWARD(in_args)...).
    /// --------------------------------------------------------------------------------------------
    template <RLogArg... TArgs>
    inline void STATIC_LOG_FATAL(LogString<TArgs...> in_msg, TArgs&&... in_args)
    {
        STATIC_LOG<ELogLevel::Fatal>(in_msg, FORWARD(in_args)...);
    }
}