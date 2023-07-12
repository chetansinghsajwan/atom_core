#pragma once
#include "Atom/Logging/Logger.h"
#include "Atom/Logging/SimpleLogger.h"

namespace Atom::Logging
{
    class LoggerFactory
    {
    public:
        LoggerPtr CreateLogger(Str name) noex
        {
            return MAKE_LOGGER<SimpleLoggerST>(MOVE(name));
        }
    };

    inline LoggerFactory& GET_LOGGER_FACTORY() noex
    {
        static LoggerFactory instance;
        return instance;
    }

    inline LoggerPtr CREATE_LOGGER(auto&&... args) noex
    {
        return GET_LOGGER_FACTORY().CreateLogger(FORWARD(args)...);
    }
}