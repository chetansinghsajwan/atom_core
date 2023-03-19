#pragma once
#include "Atom/Exceptions.h"
#include "Atom/String.h"
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
        using Container = UnorderedMap<String, LoggerPtr>;
        using Iterator = Container::iterator;
        using ConstIterator = Container::const_iterator;

    public:
        LoggerRegistry() noexcept
        {
            SetDefaultLogger(GET_LOGGER_FACTORY().CreateLogger(TEXT("DefaultLogger")));
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// Registers Logger with its name.
        /// 
        /// @PARAM[IN] logger Logger to register.
        /// 
        /// @THROWS NullPointerException Asserts {logger != nullptr}.
        /// @THROWS InvalidArgumentException Asserts {!key.empty()}, {key = logger->Name()}.
        /// @THROWS InvalidOperationException Asserts {!HasLogger(key)}, {key = logger->Name()}.
        /// 
        /// @EXCEPTION_SAFETY VeryStrong
        /// ----------------------------------------------------------------------------------------
        void RegisterLogger(LoggerPtr logger)
        {
            ASSERT<NullPointerException>(logger != nullptr,
                TEXT("Cannot register NULL Logger."));

            StringView key = logger->Name();

            ASSERT<InvalidArgumentException>(!key.empty(),
                TEXT("Cannot register logger with NULL key."));

            ASSERT<InvalidOperationException>(_HasLogger(key) == false,
                TEXT("Logger for key{{key}} is already registered."));

            _RegisterLogger(logger, String(key));
        }

        /// ----------------------------------------------------------------------------------------
        /// Registers Logger with specified key. This key then can be used to retrieve the 
        /// logger back.
        /// 
        /// If a logger with this key is already registered throws exception.
        /// Use UnregisterLogger of ForceRegisterLogger to register logger in that case.
        /// 
        /// @PARAM[IN] logger Logger to register.
        /// @PARAM[IN] key String used as key to register logger.
        /// 
        /// @THROWS NullPointerException Asserts {logger != nullptr}.
        /// @THROWS InvalidArgumentException Asserts {!key.empty()}.
        /// @THROWS InvalidOperationException Asserts {!HasLogger(key)}.
        /// 
        /// @EXCEPTION_SAFETY VeryStrong
        /// ----------------------------------------------------------------------------------------
        void RegisterLogger(LoggerPtr logger, StringView key)
        {
            ASSERT<NullPointerException>(logger != nullptr,
                TEXT("Cannot register NULL Logger."));

            ASSERT<InvalidArgumentException>(!key.empty(),
                TEXT("Cannot register logger with NULL key."));

            ASSERT<InvalidOperationException>(_HasLogger(key) == false,
                TEXT("Logger for key{{key}} is already registered."));

            _RegisterLogger(logger, String(key));
        }

        /// ----------------------------------------------------------------------------------------
        /// Overload for void RegisterLogger(LoggerPtr logger, StringView key) for performance.
        /// 
        /// @PARAM[IN] key RValue reference to String, to avoid allocating memory to store the key.
        /// ----------------------------------------------------------------------------------------
        void RegisterLogger(LoggerPtr logger, String&& key)
        {
            ASSERT<NullPointerException>(logger != nullptr,
                TEXT("Cannot register NULL Logger."));

            ASSERT<InvalidArgumentException>(!key.empty(),
                TEXT("Cannot register logger with NULL key."));

            ASSERT<InvalidOperationException>(_HasLogger(key) == false,
                TEXT("Logger for key{{key}} is already registered."));

            _RegisterLogger(logger, MOVE(key));
        }

        /// ----------------------------------------------------------------------------------------
        /// Force registers logger with its name. If a logger with key {key} is already 
        /// registered, then unregisters it and registers this.
        /// 
        /// @THROWS NullPointerException Asserts {logger != nullptr}.
        /// @THROWS InvalidArgumentException Asserts {!key.empty()}, {key = logger->Name()}.
        /// 
        /// @EXCEPTION_SAFETY VeryStrong
        /// ----------------------------------------------------------------------------------------
        void ForceRegisterLogger(LoggerPtr logger)
        {
            ASSERT<NullPointerException>(logger != nullptr,
                TEXT("Cannot register NULL Logger."));

            StringView key = logger->Name();

            ASSERT<InvalidArgumentException>(!key.empty(),
                TEXT("Cannot register logger with NULL key."));

            _ForceRegisterLogger(logger, String(key));
        }

        /// ----------------------------------------------------------------------------------------
        /// Force registers logger with specified key. If a logger with key {key} is already 
        /// registered, then unregisters it and registers this.
        /// 
        /// @PARAM[IN] logger Logger to register.
        /// @PARAM[IN] key String used as key to register logger.
        /// 
        /// @THROWS NullPointerException Asserts {logger != nullptr}.
        /// @THROWS InvalidArgumentException Asserts {!key.empty()}.
        /// 
        /// @EXCEPTION_SAFETY VeryStrong
        /// ----------------------------------------------------------------------------------------
        void ForceRegisterLogger(LoggerPtr logger, StringView key)
        {
            ASSERT<NullPointerException>(logger != nullptr,
                TEXT("Cannot register NULL Logger."));

            ASSERT<InvalidArgumentException>(!key.empty(),
                TEXT("Cannot register logger with NULL key."));

            _ForceRegisterLogger(logger, String(key));
        }

        /// ----------------------------------------------------------------------------------------
        /// Overload for void ForceRegisterLogger(LoggerPtr logger, StringView key) for performance.
        /// 
        /// @PARAM[IN] key RValue reference to String, to avoid allocating memory to store the key.
        /// ----------------------------------------------------------------------------------------
        void ForceRegisterLogger(LoggerPtr logger, String&& key)
        {
            ASSERT<NullPointerException>(logger != nullptr,
                TEXT("Cannot register NULL Logger."));

            ASSERT<InvalidArgumentException>(!key.empty(),
                TEXT("Cannot register logger with NULL key."));

            _ForceRegisterLogger(logger, MOVE(key));
        }

        /// ----------------------------------------------------------------------------------------
        /// Tries to register logger with its name.
        /// 
        /// @PARAM[IN] logger Logger to register.
        /// ----------------------------------------------------------------------------------------
        bool TryRegisterLogger(LoggerPtr logger) noexcept
        {
            if (logger == nullptr)
                return false;

            StringView key = logger->Name();
            if (key.empty())
                return false;

            if (_HasLogger(key))
                return false;

            _RegisterLogger(logger, String(key));
            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// Tries to register logger with specified key.
        /// 
        /// @PARAM[IN] logger Logger to register.
        /// @PARAM[IN] key String used as key to register logger.
        /// ----------------------------------------------------------------------------------------
        bool TryRegisterLogger(LoggerPtr logger, StringView key) noexcept
        {
            if (logger == nullptr)
                return false;

            if (key.empty())
                return false;

            if (_HasLogger(key))
                return false;

            _RegisterLogger(logger, String(key));
            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// Overload for bool TryRegisterLogger(LoggerPtr logger, StringView key) noexcept for 
        /// performance.
        /// 
        /// @PARAM[IN] key RValue reference to String, to avoid allocating memory to store the key.
        /// ----------------------------------------------------------------------------------------
        bool TryRegisterLogger(LoggerPtr logger, String&& key) noexcept
        {
            if (logger == nullptr)
                return false;

            if (key.empty())
                return false;

            if (_HasLogger(key))
                return false;

            _RegisterLogger(logger, MOVE(key));
            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// Unregisters the logger registered with key {key}.
        /// 
        /// @THROWS InvalidArgumentException Asserts {!key.empty()}.
        /// 
        /// @EXCEPTION_SAFETY VeryStrong
        /// ----------------------------------------------------------------------------------------
        bool UnregisterLogger(StringView key)
        {
            ASSERT<InvalidArgumentException>(!key.empty(),
                TEXT("Cannot access logger with NULL key."));

            return _UnregisterLogger(key) != nullptr;
        }

        /// ----------------------------------------------------------------------------------------
        /// Unregisters all loggers.
        /// ----------------------------------------------------------------------------------------
        void UnregisterAllLoggers()
        {
            _loggers.clear();
        }

        /// ----------------------------------------------------------------------------------------
        /// Unregisters and returns the logger registered with key {key}.
        /// 
        /// @PARAM[IN] key Key used to register the logger.
        /// @RETURNS Logger registered with key {key}. If no logger was registered returns nullptr.
        /// 
        /// @THROWS InvalidArgumentException Asserts {!key.empty()}.
        /// 
        /// @EXCEPTION_SAFETY VeryStrong
        /// ----------------------------------------------------------------------------------------
        LoggerPtr UnregisterAndGetLogger(StringView key)
        {
            ASSERT<InvalidArgumentException>(!key.empty(),
                TEXT("Cannot access logger with NULL key."));

            return _UnregisterLogger(key);
        }

        /// ----------------------------------------------------------------------------------------
        /// Get the logger registered with key {key}.
        /// ----------------------------------------------------------------------------------------
        LoggerPtr GetLogger(StringView key) const noexcept
        {
            ASSERT<InvalidArgumentException>(!key.empty(),
                TEXT("Cannot access logger with NULL key."));

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
        bool HasLogger(StringView key) const noexcept
        {
            ASSERT<InvalidArgumentException>(!key.empty(),
                TEXT("Cannot access logger with NULL key."));

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
        LoggerPtr GetDefaultLogger() const noexcept
        {
            return _defaultLogger;
        }

        /// ----------------------------------------------------------------------------------------
        /// Set the default logger.
        /// 
        /// @PARAM[IN] logger Logger to set as default logger. If null sets a fake logger instead.
        /// ----------------------------------------------------------------------------------------
        void SetDefaultLogger(LoggerPtr logger) noexcept
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
        /// @RETURNS ConstIterator to first Key-Logger pair.
        /// ----------------------------------------------------------------------------------------
        ConstIterator begin() const noexcept
        {
            return _loggers.begin();
        }

        /// ----------------------------------------------------------------------------------------
        /// @RETURNS ConstIterator to last Key-Logger pair. 
        /// ----------------------------------------------------------------------------------------
        ConstIterator end() const noexcept
        {
            return _loggers.cend();
        }

        /// ----------------------------------------------------------------------------------------
        /// @RETURNS ConstIterator to first Key-Logger pair.
        /// ----------------------------------------------------------------------------------------
        ConstIterator cbegin() const noexcept
        {
            return _loggers.cbegin();
        }

        /// ----------------------------------------------------------------------------------------
        /// @RETURNS ConstIterator to last Key-Logger pair.
        /// ----------------------------------------------------------------------------------------
        ConstIterator cend() const noexcept
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
        void _RegisterLogger(LoggerPtr logger, String key)
        {
            ATOM_DEBUG_EXPECTS(logger != nullptr);
            ATOM_DEBUG_EXPECTS(!key.empty());

            bool result = _loggers.insert({ MOVE(key), MOVE(logger) }).second;
            ATOM_DEBUG_EXPECTS(result);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// @EXCEPTION_SAFETY VeryStrong
        /// ----------------------------------------------------------------------------------------
        void _ForceRegisterLogger(LoggerPtr logger, String key)
        {
            ATOM_DEBUG_EXPECTS(logger != nullptr);
            ATOM_DEBUG_EXPECTS(!key.empty());

            _loggers.insert_or_assign(MOVE(key), MOVE(logger));
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// @EXCEPTION_SAFETY VeryStrong
        /// ----------------------------------------------------------------------------------------
        LoggerPtr _UnregisterLogger(StringView key)
        {
            auto it = _FindEntry(key);
            if (it == end())
            {
                return nullptr;
            }

            LoggerPtr logger = MOVE(it->second);
            _loggers.erase(it);
            return logger;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        bool _HasLogger(StringView key) const noexcept
        {
            ATOM_DEBUG_EXPECTS_MSG(!key.empty(),
                TEXT("Cannot access logger with NULL key."));

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
        ConstIterator _FindEntry(StringView key) const noexcept
        {
            auto it = _loggers.cbegin();
            for (; it != _loggers.cend(); it++)
            {
                if (it->first == key)
                {
                    break;
                }
            }

            return it;
        }

    protected:
        Container _loggers;
        LoggerPtr _defaultLogger;
    };

    inline LoggerRegistry& GET_REGISTRY() noexcept
    {
        static LoggerRegistry instance;
        return instance;
    }
}