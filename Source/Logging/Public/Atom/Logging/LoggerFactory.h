#pragma once
#include "Atom/Logging/Logger.h"
#include "Atom/Logging/SimpleLogger.h"

namespace Atom::Logging
{
    class LoggerFactory
    {
    public:
        LoggerPtr CreateLogger(String name) noexcept
        {
            return MakeShared<SimpleLoggerST>(MOVE(name));
        }
    };

    LoggerFactory& GET_LOGGER_FACTORY() noexcept
    {
        static LoggerFactory instance;
        return instance;
    }

    LoggerPtr CREATE_LOGGER(auto&&... args) noexcept
    {
        return GET_LOGGER_FACTORY().CreateLogger(FWD(args)...);
    }
}