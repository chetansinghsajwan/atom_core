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
        /// @PARAM[IN] in_logger Logger to register.
        /// 
        /// @THROWS NullPointerException Asserts {in_logger != nullptr}.
        /// @THROWS InvalidArgumentException Asserts {!key.empty()}, {key = in_logger->Name()}.
        /// @THROWS InvalidOperationException Asserts {!HasLogger(key)}, {key = in_logger->Name()}.
        /// 
        /// @EXCEPTION_SAFETY VeryStrong
        /// ----------------------------------------------------------------------------------------
        void RegisterLogger(LoggerPtr in_logger)
        {
            ASSERT<NullPointerException>(in_logger != nullptr,
                TEXT("Cannot register NULL Logger."));

            StringView key = in_logger->Name();

            ASSERT<InvalidArgumentException>(!key.empty(),
                TEXT("Cannot register in_logger with NULL key."));

            ASSERT<InvalidOperationException>(m_HasLogger(key) == false,
                TEXT("Logger for key{{key}} is already registered."));

            m_RegisterLogger(in_logger, String(key));
        }

        /// ----------------------------------------------------------------------------------------
        /// Registers Logger with specified key. This key then can be used to retrieve the 
        /// logger back.
        /// 
        /// If a logger with this key is already registered throws exception.
        /// Use UnregisterLogger of ForceRegisterLogger to register logger in that case.
        /// 
        /// @PARAM[IN] in_logger Logger to register.
        /// @PARAM[IN] in_key String used as key to register logger.
        /// 
        /// @THROWS NullPointerException Asserts {in_logger != nullptr}.
        /// @THROWS InvalidArgumentException Asserts {!in_key.empty()}.
        /// @THROWS InvalidOperationException Asserts {!HasLogger(in_key)}.
        /// 
        /// @EXCEPTION_SAFETY VeryStrong
        /// ----------------------------------------------------------------------------------------
        void RegisterLogger(LoggerPtr in_logger, StringView in_key)
        {
            ASSERT<NullPointerException>(in_logger != nullptr,
                TEXT("Cannot register NULL Logger."));

            ASSERT<InvalidArgumentException>(!in_key.empty(),
                TEXT("Cannot register in_logger with NULL key."));

            ASSERT<InvalidOperationException>(m_HasLogger(in_key) == false,
                TEXT("Logger for key{{key}} is already registered."));

            m_RegisterLogger(in_logger, String(in_key));
        }

        /// ----------------------------------------------------------------------------------------
        /// Overload for void RegisterLogger(LoggerPtr in_logger, StringView key) for performance.
        /// 
        /// @PARAM[IN] key RValue reference to String, to avoid allocating memory to store the key.
        /// ----------------------------------------------------------------------------------------
        void RegisterLogger(LoggerPtr in_logger, String&& in_key)
        {
            ASSERT<NullPointerException>(in_logger != nullptr,
                TEXT("Cannot register NULL Logger."));

            ASSERT<InvalidArgumentException>(!in_key.empty(),
                TEXT("Cannot register in_logger with NULL key."));

            ASSERT<InvalidOperationException>(m_HasLogger(in_key) == false,
                TEXT("Logger for key{{key}} is already registered."));

            m_RegisterLogger(in_logger, MOVE(in_key));
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
        void ForceRegisterLogger(LoggerPtr in_logger)
        {
            ASSERT<NullPointerException>(in_logger != nullptr,
                TEXT("Cannot register NULL Logger."));

            StringView key = in_logger->Name();

            ASSERT<InvalidArgumentException>(!key.empty(),
                TEXT("Cannot register logger with NULL key."));

            m_ForceRegisterLogger(in_logger, String(key));
        }

        /// ----------------------------------------------------------------------------------------
        /// Force registers logger with specified key. If a logger with key {key} is already 
        /// registered, then unregisters it and registers this.
        /// 
        /// @PARAM[IN] in_logger Logger to register.
        /// @PARAM[IN] key String used as key to register logger.
        /// 
        /// @THROWS NullPointerException Asserts {in_logger != nullptr}.
        /// @THROWS InvalidArgumentException Asserts {!key.empty()}.
        /// 
        /// @EXCEPTION_SAFETY VeryStrong
        /// ----------------------------------------------------------------------------------------
        void ForceRegisterLogger(LoggerPtr in_logger, StringView key)
        {
            ASSERT<NullPointerException>(in_logger != nullptr,
                TEXT("Cannot register NULL Logger."));

            ASSERT<InvalidArgumentException>(!key.empty(),
                TEXT("Cannot register logger with NULL key."));

            m_ForceRegisterLogger(in_logger, String(key));
        }

        /// ----------------------------------------------------------------------------------------
        /// Overload for void ForceRegisterLogger(LoggerPtr in_logger, StringView key) for performance.
        /// 
        /// @PARAM[IN] key RValue reference to String, to avoid allocating memory to store the key.
        /// ----------------------------------------------------------------------------------------
        void ForceRegisterLogger(LoggerPtr in_logger, String&& in_key)
        {
            ASSERT<NullPointerException>(in_logger != nullptr,
                TEXT("Cannot register NULL Logger."));

            ASSERT<InvalidArgumentException>(!in_key.empty(),
                TEXT("Cannot register logger with NULL key."));

            m_ForceRegisterLogger(in_logger, MOVE(in_key));
        }

        /// ----------------------------------------------------------------------------------------
        /// Tries to register logger with its name.
        /// 
        /// @PARAM[IN] in_logger Logger to register.
        /// ----------------------------------------------------------------------------------------
        bool TryRegisterLogger(LoggerPtr in_logger) noexcept
        {
            if (in_logger == nullptr)
                return false;

            StringView key = in_logger->Name();
            if (key.empty())
                return false;

            if (m_HasLogger(key))
                return false;

            m_RegisterLogger(in_logger, String(key));
            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// Tries to register logger with specified key.
        /// 
        /// @PARAM[IN] in_logger Logger to register.
        /// @PARAM[IN] in_key String used as key to register logger.
        /// ----------------------------------------------------------------------------------------
        bool TryRegisterLogger(LoggerPtr in_logger, StringView in_key) noexcept
        {
            if (in_logger == nullptr)
                return false;

            if (in_key.empty())
                return false;

            if (m_HasLogger(in_key))
                return false;

            m_RegisterLogger(in_logger, String(in_key));
            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// Overload for bool TryRegisterLogger(LoggerPtr in_logger, StringView key) noexcept for 
        /// performance.
        /// 
        /// @PARAM[IN] key RValue reference to String, to avoid allocating memory to store the key.
        /// ----------------------------------------------------------------------------------------
        bool TryRegisterLogger(LoggerPtr in_logger, String&& in_key) noexcept
        {
            if (in_logger == nullptr)
                return false;

            if (in_key.empty())
                return false;

            if (m_HasLogger(in_key))
                return false;

            m_RegisterLogger(in_logger, MOVE(in_key));
            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// Unregisters the logger registered with key {in_key}.
        /// 
        /// @THROWS InvalidArgumentException Asserts {!in_key.empty()}.
        /// 
        /// @EXCEPTION_SAFETY VeryStrong
        /// ----------------------------------------------------------------------------------------
        bool UnregisterLogger(StringView in_key)
        {
            ASSERT<InvalidArgumentException>(!in_key.empty(),
                TEXT("Cannot access logger with NULL key."));

            return m_UnregisterLogger(in_key) != nullptr;
        }

        /// ----------------------------------------------------------------------------------------
        /// Unregisters all loggers.
        /// ----------------------------------------------------------------------------------------
        void UnregisterAllLoggers()
        {
            m_loggers.clear();
        }

        /// ----------------------------------------------------------------------------------------
        /// Unregisters and returns the logger registered with key {in_key}.
        /// 
        /// @PARAM[IN] in_key Key used to register the logger.
        /// @RETURNS Logger registered with key {in_key}. If no logger was registered returns nullptr.
        /// 
        /// @THROWS InvalidArgumentException Asserts {!in_key.empty()}.
        /// 
        /// @EXCEPTION_SAFETY VeryStrong
        /// ----------------------------------------------------------------------------------------
        LoggerPtr UnregisterAndGetLogger(StringView in_key)
        {
            ASSERT<InvalidArgumentException>(!in_key.empty(),
                TEXT("Cannot access logger with NULL key."));

            return m_UnregisterLogger(in_key);
        }

        /// ----------------------------------------------------------------------------------------
        /// Get the logger registered with key {key}.
        /// ----------------------------------------------------------------------------------------
        LoggerPtr GetLogger(StringView in_key) const noexcept
        {
            ASSERT<InvalidArgumentException>(!in_key.empty(),
                TEXT("Cannot access logger with NULL key."));

            for (auto pair : m_loggers)
            {
                if (pair.first == in_key)
                {
                    return pair.second;
                }
            }

            return nullptr;
        }

        /// ----------------------------------------------------------------------------------------
        /// Checks if a logger for key {in_key} is registered.
        /// ----------------------------------------------------------------------------------------
        bool HasLogger(StringView in_key) const noexcept
        {
            ASSERT<InvalidArgumentException>(!in_key.empty(),
                TEXT("Cannot access logger with NULL key."));

            return m_HasLogger(in_key);
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
        /// @PARAM[IN] in_logger Logger to set as default logger. If null sets a fake logger instead.
        /// ----------------------------------------------------------------------------------------
        void SetDefaultLogger(LoggerPtr in_logger) noexcept
        {
            if (in_logger == nullptr)
            {
                in_logger = NullLogger::Instance;
            }

            m_defaultLogger = in_logger;
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
            return m_loggers.begin();
        }

        /// ----------------------------------------------------------------------------------------
        /// @RETURNS ConstIterator to last Key-Logger pair. 
        /// ----------------------------------------------------------------------------------------
        ConstIterator end() const noexcept
        {
            return m_loggers.cend();
        }

        /// ----------------------------------------------------------------------------------------
        /// @RETURNS ConstIterator to first Key-Logger pair.
        /// ----------------------------------------------------------------------------------------
        ConstIterator cbegin() const noexcept
        {
            return m_loggers.cbegin();
        }

        /// ----------------------------------------------------------------------------------------
        /// @RETURNS ConstIterator to last Key-Logger pair.
        /// ----------------------------------------------------------------------------------------
        ConstIterator cend() const noexcept
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
        void m_RegisterLogger(LoggerPtr in_logger, String in_key)
        {
            ATOM_DEBUG_EXPECTS(in_logger != nullptr);
            ATOM_DEBUG_EXPECTS(!in_key.empty());

            bool result = m_loggers.insert({ MOVE(in_key), MOVE(in_logger) }).second;
            ATOM_DEBUG_EXPECTS(result);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// @EXCEPTION_SAFETY VeryStrong
        /// ----------------------------------------------------------------------------------------
        void m_ForceRegisterLogger(LoggerPtr in_logger, String in_key)
        {
            ATOM_DEBUG_EXPECTS(in_logger != nullptr);
            ATOM_DEBUG_EXPECTS(!in_key.empty());

            m_loggers.insert_or_assign(MOVE(in_key), MOVE(in_logger));
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// @EXCEPTION_SAFETY VeryStrong
        /// ----------------------------------------------------------------------------------------
        LoggerPtr m_UnregisterLogger(StringView in_key)
        {
            auto it = m_FindEntry(in_key);
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
        bool m_HasLogger(StringView in_key) const noexcept
        {
            ATOM_DEBUG_EXPECTS_MSG(!in_key.empty(),
                TEXT("Cannot access logger with NULL key."));

            for (auto pair : m_loggers)
            {
                if (pair.first == in_key)
                {
                    return true;
                }
            }

            return false;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        ConstIterator m_FindEntry(StringView in_key) const noexcept
        {
            auto it = m_loggers.cbegin();
            for (; it != m_loggers.cend(); it++)
            {
                if (it->first == in_key)
                {
                    break;
                }
            }

            return it;
        }

    protected:
        Container m_loggers;
        LoggerPtr m_defaultLogger;
    };

    inline LoggerRegistry& GET_REGISTRY() noexcept
    {
        static LoggerRegistry s_instance;
        return s_instance;
    }
}