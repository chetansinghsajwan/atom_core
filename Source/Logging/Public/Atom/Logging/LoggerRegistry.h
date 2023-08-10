#pragma once
#include "Atom/Exceptions.h"
#include "Atom/Str.h"
#include "Atom/Containers.h"
#include "Atom/Logging/Logger.h"
#include "Atom/Logging/NullLogger.h"
#include "Atom/Logging/LoggerFactory.h"

namespace Atom::Logging
{
    /// --------------------------------------------------------------------------------------------
    /// LoggerRegistry is used manage loggers collectively and provides interface to register 
    /// loggers with a key.
    /// 
    /// @TODO Add thread safety.
    /// @TODO Add string like template parameter support.
    /// --------------------------------------------------------------------------------------------
    class LoggerRegistry
    {
        using TContainer = UnorderedMap<Str, LoggerPtr>;
        using TIter = typename TContainer::const_iterator;
        using TIterEnd = typename TContainer::const_iterator;

    public:
        LoggerRegistry()
        {
            SetDefaultLogger(GET_LOGGER_FACTORY().CreateLogger("DefaultLogger"));
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// Registers Logger with its name.
        /// 
        /// @PARAM[IN] logger Logger to register.
        /// 
        /// @THROWS NullPointerException Asserts {logger != nullptr}.
        /// @THROWS InvalidArgumentException Asserts {!key.isEmpty()}, {key = logger->Name()}.
        /// @THROWS InvalidOperationException Asserts {!HasLogger(key)}, {key = logger->Name()}.
        /// 
        /// @EXCEPTION_SAFETY VeryStrong
        /// ----------------------------------------------------------------------------------------
        fn RegisterLogger(LoggerPtr logger)
        {
            ATOM_ASSERT(logger != nullptr) << NullPointerException(
                "Cannot register NULL Logger.");

            StrView key = logger->Name();

            ATOM_ASSERT(!key.isEmpty()) << InvalidArgumentException(
                "Cannot register logger with NULL key.");

            ATOM_ASSERT(_HasLogger(key) == false) << InvalidOperationException(
                "Logger for key{{key}} is already registered.");

            _RegisterLogger(logger, Str(key));
        }

        /// ----------------------------------------------------------------------------------------
        /// Registers Logger with specified key. This key then can be used to retrieve the 
        /// logger back.
        /// 
        /// If a logger with this key is already registered throws exception.
        /// Use UnregisterLogger of ForceRegisterLogger to register logger in that case.
        /// 
        /// @PARAM[IN] logger Logger to register.
        /// @PARAM[IN] key Str used as key to register logger.
        /// 
        /// @THROWS NullPointerException Asserts {logger != nullptr}.
        /// @THROWS InvalidArgumentException Asserts {!key.isEmpty()}.
        /// @THROWS InvalidOperationException Asserts {!HasLogger(key)}.
        /// 
        /// @EXCEPTION_SAFETY VeryStrong
        /// ----------------------------------------------------------------------------------------
        fn RegisterLogger(LoggerPtr logger, StrView key)
        {
            ATOM_ASSERT(logger != nullptr) << NullPointerException(
                "Cannot register NULL Logger.");

            ATOM_ASSERT(!key.isEmpty()) << InvalidArgumentException(
                "Cannot register logger with NULL key.");

            ATOM_ASSERT(_HasLogger(key) == false) << InvalidOperationException(
                "Logger for key{{key}} is already registered.");

            _RegisterLogger(logger, Str(key));
        }

        /// ----------------------------------------------------------------------------------------
        /// Overload for void RegisterLogger(LoggerPtr logger, StrView key) for performance.
        /// 
        /// @PARAM[IN] key RValue reference to Str, to avoid allocating memory to store the key.
        /// ----------------------------------------------------------------------------------------
        fn RegisterLogger(LoggerPtr logger, Str&& key)
        {
            ATOM_ASSERT(logger != nullptr) << NullPointerException(
                "Cannot register NULL Logger.");

            ATOM_ASSERT(!key.isEmpty()) << InvalidArgumentException(
                "Cannot register logger with NULL key.");

            ATOM_ASSERT(_HasLogger(key) == false) << InvalidOperationException(
                "Logger for key{{key}} is already registered.");

            _RegisterLogger(logger, mov(key));
        }

        /// ----------------------------------------------------------------------------------------
        /// Force registers logger with its name. If a logger with key {key} is already 
        /// registered, then unregisters it and registers this.
        /// 
        /// @THROWS NullPointerException Asserts {logger != nullptr}.
        /// @THROWS InvalidArgumentException Asserts {!key.isEmpty()}, {key = logger->Name()}.
        /// 
        /// @EXCEPTION_SAFETY VeryStrong
        /// ----------------------------------------------------------------------------------------
        fn ForceRegisterLogger(LoggerPtr logger)
        {
            ATOM_ASSERT(logger != nullptr) << NullPointerException(
                "Cannot register NULL Logger.");

            StrView key = logger->Name();

            ATOM_ASSERT(!key.isEmpty()) << InvalidArgumentException(
                "Cannot register logger with NULL key.");

            _ForceRegisterLogger(logger, Str(key));
        }

        /// ----------------------------------------------------------------------------------------
        /// Force registers logger with specified key. If a logger with key {key} is already 
        /// registered, then unregisters it and registers this.
        /// 
        /// @PARAM[IN] logger Logger to register.
        /// @PARAM[IN] key Str used as key to register logger.
        /// 
        /// @THROWS NullPointerException Asserts {logger != nullptr}.
        /// @THROWS InvalidArgumentException Asserts {!key.isEmpty()}.
        /// 
        /// @EXCEPTION_SAFETY VeryStrong
        /// ----------------------------------------------------------------------------------------
        fn ForceRegisterLogger(LoggerPtr logger, StrView key)
        {
            ATOM_ASSERT(logger != nullptr) << NullPointerException(
                "Cannot register NULL Logger.");

            ATOM_ASSERT(!key.isEmpty()) << InvalidArgumentException(
                "Cannot register logger with NULL key.");

            _ForceRegisterLogger(logger, Str(key));
        }

        /// ----------------------------------------------------------------------------------------
        /// Overload for void ForceRegisterLogger(LoggerPtr logger, StrView key) for performance.
        /// 
        /// @PARAM[IN] key RValue reference to Str, to avoid allocating memory to store the key.
        /// ----------------------------------------------------------------------------------------
        fn ForceRegisterLogger(LoggerPtr logger, Str&& key)
        {
            ATOM_ASSERT(logger != nullptr) << NullPointerException(
                "Cannot register NULL Logger.");

            ATOM_ASSERT(!key.isEmpty()) << InvalidArgumentException(
                "Cannot register logger with NULL key.");

            _ForceRegisterLogger(logger, mov(key));
        }

        /// ----------------------------------------------------------------------------------------
        /// Tries to register logger with its name.
        /// 
        /// @PARAM[IN] logger Logger to register.
        /// ----------------------------------------------------------------------------------------
        fn TryRegisterLogger(LoggerPtr logger) -> bool
        {
            if (logger == nullptr)
                return false;

            StrView key = logger->Name();
            if (key.isEmpty())
                return false;

            if (_HasLogger(key))
                return false;

            _RegisterLogger(logger, Str(key));
            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// Tries to register logger with specified key.
        /// 
        /// @PARAM[IN] logger Logger to register.
        /// @PARAM[IN] key Str used as key to register logger.
        /// ----------------------------------------------------------------------------------------
        fn TryRegisterLogger(LoggerPtr logger, StrView key) -> bool
        {
            if (logger == nullptr)
                return false;

            if (key.isEmpty())
                return false;

            if (_HasLogger(key))
                return false;

            _RegisterLogger(logger, Str(key));
            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// Overload for bool TryRegisterLogger(LoggerPtr logger, StrView key) for 
        /// performance.
        /// 
        /// @PARAM[IN] key RValue reference to Str, to avoid allocating memory to store the key.
        /// ----------------------------------------------------------------------------------------
        fn TryRegisterLogger(LoggerPtr logger, Str&& key) -> bool
        {
            if (logger == nullptr)
                return false;

            if (key.isEmpty())
                return false;

            if (_HasLogger(key))
                return false;

            _RegisterLogger(logger, mov(key));
            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// Unregisters the logger registered with key {key}.
        /// 
        /// @THROWS InvalidArgumentException Asserts {!key.isEmpty()}.
        /// 
        /// @EXCEPTION_SAFETY VeryStrong
        /// ----------------------------------------------------------------------------------------
        fn UnregisterLogger(StrView key) -> bool
        {
            ATOM_ASSERT(!key.isEmpty()) << InvalidArgumentException(
                "Cannot access logger with NULL key.");

            return _UnregisterLogger(key) != nullptr;
        }

        /// ----------------------------------------------------------------------------------------
        /// Unregisters all loggers.
        /// ----------------------------------------------------------------------------------------
        fn UnregisterAllLoggers()
        {
            _loggers.clear();
        }

        /// ----------------------------------------------------------------------------------------
        /// Unregisters and returns the logger registered with key {key}.
        /// 
        /// @PARAM[IN] key Key used to register the logger.
        /// @RETURNS Logger registered with key {key}. If no logger was registered returns nullptr.
        /// 
        /// @THROWS InvalidArgumentException Asserts {!key.isEmpty()}.
        /// 
        /// @EXCEPTION_SAFETY VeryStrong
        /// ----------------------------------------------------------------------------------------
        fn UnregisterAndGetLogger(StrView key) -> LoggerPtr
        {
            ATOM_ASSERT(!key.isEmpty()) << InvalidArgumentException(
                "Cannot access logger with NULL key.");

            return _UnregisterLogger(key);
        }

        /// ----------------------------------------------------------------------------------------
        /// Get the logger registered with key {key}.
        /// ----------------------------------------------------------------------------------------
        fn GetLogger(StrView key) const -> LoggerPtr
        {
            for (auto pair : _loggers)
            {
                if (pair.first == key)
                {
                    return pair.second;
                }
            }

            return nullptr;
        }

        /// ----------------------------------------------------------------------------------------
        /// Checks if a logger for key {key} is registered.
        /// ----------------------------------------------------------------------------------------
        fn HasLogger(StrView key) const -> bool
        {
            return _HasLogger(key);
        }

    //// -------------------------------------------------------------------------------------------
    //// DefaultLogger
    //// 
    //// DefaultLogger is cached for faster retrieval.
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// Get the default logger
        /// In case there is no default logger assigned, returns a fake logger but never 
        /// returns nullptr.
        /// 
        /// DefaultLogger is used to log global logs or when categorization is not necessary.
        /// ----------------------------------------------------------------------------------------
        fn GetDefaultLogger() const -> LoggerPtr
        {
            return _defaultLogger;
        }

        /// ----------------------------------------------------------------------------------------
        /// Set the default logger.
        /// 
        /// @PARAM[IN] logger Logger to set as default logger. If null sets a fake logger instead.
        /// ----------------------------------------------------------------------------------------
        fn SetDefaultLogger(LoggerPtr logger) -> void
        {
            if (logger == nullptr)
            {
                logger = NullLogger::Instance;
            }

            _defaultLogger = logger;
        }

    //// -------------------------------------------------------------------------------------------
    //// Iteration
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// @RETURNS TIter to first Key-Logger pair.
        /// ----------------------------------------------------------------------------------------
        fn iter() const -> TIter
        {
            return _loggers.begin();
        }

        /// ----------------------------------------------------------------------------------------
        /// @RETURNS TIter to last Key-Logger pair. 
        /// ----------------------------------------------------------------------------------------
        fn iterEnd() const -> TIter
        {
            return _loggers.cend();
        }

    //// -------------------------------------------------------------------------------------------
    //// Implementation functions
    //// 
    //// These function generally don't throw exceptions at runtime.
    //// This promotes code usability with flexibility and no performance overhead.
    //// 
    //// However these functions have some expectations and may throw in DEBUG mode if 
    //// expectations are not met.
    //// -------------------------------------------------------------------------------------------

    protected:

        /// ----------------------------------------------------------------------------------------
        /// 
        /// 
        /// @EXCEPTION_SAFETY VeryStrong
        /// ----------------------------------------------------------------------------------------
        fn _RegisterLogger(LoggerPtr logger, Str key) -> void
        {
            ATOM_DEBUG_EXPECTS(logger != nullptr);
            ATOM_DEBUG_EXPECTS(!key.isEmpty());

            bool result = _loggers.insert({ mov(key), mov(logger) }).second;
            ATOM_DEBUG_ENSURES(result);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// @EXCEPTION_SAFETY VeryStrong
        /// ----------------------------------------------------------------------------------------
        fn _ForceRegisterLogger(LoggerPtr logger, Str key) -> void
        {
            ATOM_DEBUG_EXPECTS(logger != nullptr);
            ATOM_DEBUG_EXPECTS(!key.isEmpty());

            _loggers.insert_or_assign(mov(key), mov(logger));
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// @EXCEPTION_SAFETY VeryStrong
        /// ----------------------------------------------------------------------------------------
        fn _UnregisterLogger(StrView key) -> LoggerPtr
        {
            auto it = _FindEntry(key);
            if (it == iterEnd())
            {
                return nullptr;
            }

            LoggerPtr logger = mov(const_cast<LoggerPtr&>(it->second));
            _loggers.erase(it);
            return logger;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        fn _HasLogger(StrView key) const -> bool
        {
            for (auto pair : _loggers)
            {
                if (pair.first == key)
                {
                    return true;
                }
            }

            return false;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        fn _FindEntry(StrView key) const -> TIter
        {
            auto it = iter();
            for (; it != iterEnd(); it++)
            {
                if (it->first == key)
                {
                    break;
                }
            }

            return it;
        }

    protected:
        TContainer _loggers;
        LoggerPtr _defaultLogger;
    };

    inline fn GET_REGISTRY() -> LoggerRegistry&
    {
        static LoggerRegistry s_instance;
        return s_instance;
    }
}