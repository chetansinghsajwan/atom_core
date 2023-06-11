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
        using TIterator = typename TContainer::const_iterator;

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
        /// @THROWS InvalidArgumentException Asserts {!key.IsEmpty()}, {key = logger->Name()}.
        /// @THROWS InvalidOperationException Asserts {!HasLogger(key)}, {key = logger->Name()}.
        /// 
        /// @EXCEPTION_SAFETY VeryStrong
        /// ----------------------------------------------------------------------------------------
        void RegisterLogger(LoggerPtr logger)
        {
            ATOM_ASSERT(logger != nullptr) << NullPointerException(
                TEXT("Cannot register NULL Logger."));

            StrView key = logger->Name();

            ATOM_ASSERT(!key.IsEmpty()) << InvalidArgumentException(
                TEXT("Cannot register logger with NULL key."));

            ATOM_ASSERT(m_HasLogger(key) == false) << InvalidOperationException(
                TEXT("Logger for key{{key}} is already registered."));

            m_RegisterLogger(logger, Str(key));
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
        /// @THROWS InvalidArgumentException Asserts {!key.IsEmpty()}.
        /// @THROWS InvalidOperationException Asserts {!HasLogger(key)}.
        /// 
        /// @EXCEPTION_SAFETY VeryStrong
        /// ----------------------------------------------------------------------------------------
        void RegisterLogger(LoggerPtr logger, StrView key)
        {
            ATOM_ASSERT(logger != nullptr) << NullPointerException(
                TEXT("Cannot register NULL Logger."));

            ATOM_ASSERT(!key.IsEmpty()) << InvalidArgumentException(
                TEXT("Cannot register logger with NULL key."));

            ATOM_ASSERT(m_HasLogger(key) == false) << InvalidOperationException(
                TEXT("Logger for key{{key}} is already registered."));

            m_RegisterLogger(logger, Str(key));
        }

        /// ----------------------------------------------------------------------------------------
        /// Overload for void RegisterLogger(LoggerPtr logger, StrView key) for performance.
        /// 
        /// @PARAM[IN] key RValue reference to Str, to avoid allocating memory to store the key.
        /// ----------------------------------------------------------------------------------------
        void RegisterLogger(LoggerPtr logger, Str&& key)
        {
            ATOM_ASSERT(logger != nullptr) << NullPointerException(
                TEXT("Cannot register NULL Logger."));

            ATOM_ASSERT(!key.IsEmpty()) << InvalidArgumentException(
                TEXT("Cannot register logger with NULL key."));

            ATOM_ASSERT(m_HasLogger(key) == false) << InvalidOperationException(
                TEXT("Logger for key{{key}} is already registered."));

            m_RegisterLogger(logger, MOVE(key));
        }

        /// ----------------------------------------------------------------------------------------
        /// Force registers logger with its name. If a logger with key {key} is already 
        /// registered, then unregisters it and registers this.
        /// 
        /// @THROWS NullPointerException Asserts {logger != nullptr}.
        /// @THROWS InvalidArgumentException Asserts {!key.IsEmpty()}, {key = logger->Name()}.
        /// 
        /// @EXCEPTION_SAFETY VeryStrong
        /// ----------------------------------------------------------------------------------------
        void ForceRegisterLogger(LoggerPtr logger)
        {
            ATOM_ASSERT(logger != nullptr) << NullPointerException(
                TEXT("Cannot register NULL Logger."));

            StrView key = logger->Name();

            ATOM_ASSERT(!key.IsEmpty()) << InvalidArgumentException(
                TEXT("Cannot register logger with NULL key."));

            m_ForceRegisterLogger(logger, Str(key));
        }

        /// ----------------------------------------------------------------------------------------
        /// Force registers logger with specified key. If a logger with key {key} is already 
        /// registered, then unregisters it and registers this.
        /// 
        /// @PARAM[IN] logger Logger to register.
        /// @PARAM[IN] key Str used as key to register logger.
        /// 
        /// @THROWS NullPointerException Asserts {logger != nullptr}.
        /// @THROWS InvalidArgumentException Asserts {!key.IsEmpty()}.
        /// 
        /// @EXCEPTION_SAFETY VeryStrong
        /// ----------------------------------------------------------------------------------------
        void ForceRegisterLogger(LoggerPtr logger, StrView key)
        {
            ATOM_ASSERT(logger != nullptr) << NullPointerException(
                TEXT("Cannot register NULL Logger."));

            ATOM_ASSERT(!key.IsEmpty()) << InvalidArgumentException(
                TEXT("Cannot register logger with NULL key."));

            m_ForceRegisterLogger(logger, Str(key));
        }

        /// ----------------------------------------------------------------------------------------
        /// Overload for void ForceRegisterLogger(LoggerPtr logger, StrView key) for performance.
        /// 
        /// @PARAM[IN] key RValue reference to Str, to avoid allocating memory to store the key.
        /// ----------------------------------------------------------------------------------------
        void ForceRegisterLogger(LoggerPtr logger, Str&& key)
        {
            ATOM_ASSERT(logger != nullptr) << NullPointerException(
                TEXT("Cannot register NULL Logger."));

            ATOM_ASSERT(!key.IsEmpty()) << InvalidArgumentException(
                TEXT("Cannot register logger with NULL key."));

            m_ForceRegisterLogger(logger, MOVE(key));
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

            StrView key = logger->Name();
            if (key.IsEmpty())
                return false;

            if (m_HasLogger(key))
                return false;

            m_RegisterLogger(logger, Str(key));
            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// Tries to register logger with specified key.
        /// 
        /// @PARAM[IN] logger Logger to register.
        /// @PARAM[IN] key Str used as key to register logger.
        /// ----------------------------------------------------------------------------------------
        bool TryRegisterLogger(LoggerPtr logger, StrView key) noexcept
        {
            if (logger == nullptr)
                return false;

            if (key.IsEmpty())
                return false;

            if (m_HasLogger(key))
                return false;

            m_RegisterLogger(logger, Str(key));
            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// Overload for bool TryRegisterLogger(LoggerPtr logger, StrView key) noexcept for 
        /// performance.
        /// 
        /// @PARAM[IN] key RValue reference to Str, to avoid allocating memory to store the key.
        /// ----------------------------------------------------------------------------------------
        bool TryRegisterLogger(LoggerPtr logger, Str&& key) noexcept
        {
            if (logger == nullptr)
                return false;

            if (key.IsEmpty())
                return false;

            if (m_HasLogger(key))
                return false;

            m_RegisterLogger(logger, MOVE(key));
            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// Unregisters the logger registered with key {key}.
        /// 
        /// @THROWS InvalidArgumentException Asserts {!key.IsEmpty()}.
        /// 
        /// @EXCEPTION_SAFETY VeryStrong
        /// ----------------------------------------------------------------------------------------
        bool UnregisterLogger(StrView key)
        {
            ATOM_ASSERT(!key.IsEmpty()) << InvalidArgumentException(
                TEXT("Cannot access logger with NULL key."));

            return m_UnregisterLogger(key) != nullptr;
        }

        /// ----------------------------------------------------------------------------------------
        /// Unregisters all loggers.
        /// ----------------------------------------------------------------------------------------
        void UnregisterAllLoggers()
        {
            m_loggers.clear();
        }

        /// ----------------------------------------------------------------------------------------
        /// Unregisters and returns the logger registered with key {key}.
        /// 
        /// @PARAM[IN] key Key used to register the logger.
        /// @RETURNS Logger registered with key {key}. If no logger was registered returns nullptr.
        /// 
        /// @THROWS InvalidArgumentException Asserts {!key.IsEmpty()}.
        /// 
        /// @EXCEPTION_SAFETY VeryStrong
        /// ----------------------------------------------------------------------------------------
        LoggerPtr UnregisterAndGetLogger(StrView key)
        {
            ATOM_ASSERT(!key.IsEmpty()) << InvalidArgumentException(
                TEXT("Cannot access logger with NULL key."));

            return m_UnregisterLogger(key);
        }

        /// ----------------------------------------------------------------------------------------
        /// Get the logger registered with key {key}.
        /// ----------------------------------------------------------------------------------------
        LoggerPtr GetLogger(StrView key) const noexcept
        {
            for (auto pair : m_loggers)
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
        bool HasLogger(StrView key) const noexcept
        {
            return m_HasLogger(key);
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
            return m_defaultLogger;
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

            m_defaultLogger = logger;
        }

    //// -------------------------------------------------------------------------------------------
    //// Iteration
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// @RETURNS TIterator to first Key-Logger pair.
        /// ----------------------------------------------------------------------------------------
        TIterator begin() const noexcept
        {
            return m_loggers.begin();
        }

        /// ----------------------------------------------------------------------------------------
        /// @RETURNS TIterator to last Key-Logger pair. 
        /// ----------------------------------------------------------------------------------------
        TIterator end() const noexcept
        {
            return m_loggers.cend();
        }

        /// ----------------------------------------------------------------------------------------
        /// @RETURNS TIterator to first Key-Logger pair.
        /// ----------------------------------------------------------------------------------------
        TIterator cbegin() const noexcept
        {
            return m_loggers.cbegin();
        }

        /// ----------------------------------------------------------------------------------------
        /// @RETURNS TIterator to last Key-Logger pair.
        /// ----------------------------------------------------------------------------------------
        TIterator cend() const noexcept
        {
            return m_loggers.cend();
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
        void m_RegisterLogger(LoggerPtr logger, Str key)
        {
            ATOM_DEBUG_EXPECTS(logger != nullptr);
            ATOM_DEBUG_EXPECTS(!key.IsEmpty());

            bool result = m_loggers.insert({ MOVE(key), MOVE(logger) }).second;
            ATOM_DEBUG_EXPECTS(result);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// @EXCEPTION_SAFETY VeryStrong
        /// ----------------------------------------------------------------------------------------
        void m_ForceRegisterLogger(LoggerPtr logger, Str key)
        {
            ATOM_DEBUG_EXPECTS(logger != nullptr);
            ATOM_DEBUG_EXPECTS(!key.IsEmpty());

            m_loggers.insert_or_assign(MOVE(key), MOVE(logger));
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// @EXCEPTION_SAFETY VeryStrong
        /// ----------------------------------------------------------------------------------------
        LoggerPtr m_UnregisterLogger(StrView key)
        {
            auto it = m_FindEntry(key);
            if (it == end())
            {
                return nullptr;
            }

            LoggerPtr logger = MOVE(it->second);
            m_loggers.erase(it);
            return logger;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        bool m_HasLogger(StrView key) const noexcept
        {
            for (auto pair : m_loggers)
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
        TIterator m_FindEntry(StrView key) const noexcept
        {
            auto it = m_loggers.cbegin();
            for (; it != m_loggers.cend(); it++)
            {
                if (it->first == key)
                {
                    break;
                }
            }

            return it;
        }

    protected:
        TContainer m_loggers;
        LoggerPtr m_defaultLogger;
    };

    inline LoggerRegistry& GET_REGISTRY() noexcept
    {
        static LoggerRegistry s_instance;
        return s_instance;
    }
}