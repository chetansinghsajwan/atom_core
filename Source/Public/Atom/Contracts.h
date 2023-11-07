#pragma once
#include "Atom/Core.h"
#include "Atom/Core/SourceLineInfo.h"
#include "Contracts.decl.h"

#include "fmt/format.h"
#include <iostream>

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    enum class ContractType
    {
        PreCondition,
        Assertion,
        PostCondition,
        DebugPreCondition,
        DebugAssertion,
        DebugPostCondition
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    class ContractViolation
    {
    public:
        ContractType type;
        std::string_view msg;
        SourceLineInfo src;
    };

    constexpr auto _ContractTypeToString(ContractType type) -> std::string_view
    {
        switch (type)
        {
            case ContractType::PreCondition:       return "PreCondition";
            case ContractType::Assertion:          return "Assertion";
            case ContractType::PostCondition:      return "PostCondition";
            case ContractType::DebugPreCondition:  return "DebugPreCondition";
            case ContractType::DebugAssertion:     return "DebugAssertion";
            case ContractType::DebugPostCondition: return "DebugPostCondition";
            default:                               return "[INVALID_VALUE]";
        }
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    class ContractViolationException: public std::exception
    {
    public:
        ContractViolationException(ContractViolation violation):
            violation{ violation }
        {
            _what = fmt::format("Contracts {} Violation:"
                                "\n\twith msg: {}"
                                "\n\tat: {}: {}: {}"
                                "\n\tfunc: {}",
                _ContractTypeToString(violation.type), violation.msg,
                violation.src.fileName, violation.src.line.val(), violation.src.column.val(),
                violation.src.funcName);
        }

    public:
        virtual auto what() const noexcept -> const char* override
        {
            return _what.data();
        }

    public:
        ContractViolation violation;

    private:
        std::string _what;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    class ContractViolationHandler
    {
    public:
        virtual auto handle(const ContractViolation& violation) -> void = 0;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    class DefaultContractViolationHandler final: public ContractViolationHandler
    {
    public:
        virtual auto handle(const ContractViolation& violation) -> void override
        {
            if constexpr (BuildConfig::IsModeDebug())
            {
                throw ContractViolationException{ violation };
            }
            else
            {
                std::cout << "Contracts " << _ContractTypeToString(violation.type) << " Violation:"
                          << "\n\twith msg: " << violation.msg << "'"
                          << "\n\tat: " << violation.src.fileName << ":" << violation.src.line.val()
                          << ":" << violation.src.column.val() << ": " << violation.src.funcName
                          << std::endl;

                std::terminate();
            }
        }
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    class ContractViolationHandlerManager
    {
    public:
        static auto GetHandler() -> ContractViolationHandler&
        {
            return *_Handler;
        }

        static auto SetHandler(ContractViolationHandler* handler)
        {
            _Handler = handler;
            if (_Handler == nullptr)
            {
                _Handler = &_DefaultHandler;
            }
        }

        static auto SetHandlerToDefault()
        {
            _Handler = &_DefaultHandler;
        }

    private:
        static DefaultContractViolationHandler _DefaultHandler;
        static ContractViolationHandler* _Handler;
    };

    inline DefaultContractViolationHandler ContractViolationHandlerManager::_DefaultHandler =
        DefaultContractViolationHandler();
    inline ContractViolationHandler* ContractViolationHandlerManager::_Handler = &_DefaultHandler;

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    inline auto _ContractCheckImpl(
        _ContractType type, std::source_location src, std::string_view msg) -> void
    {
        SourceLineInfo srcInfo{ .line = src.line(),
            .column = src.column(),
            .funcName = src.function_name(),
            .fileName = src.file_name() };

        ContractViolation violation{ .type = ContractType(type), .msg = msg, .src = srcInfo };

        ContractViolationHandlerManager::GetHandler().handle(violation);
    }

    inline auto _Panic(std::source_location src, std::string_view msg) -> void
    {
        std::cerr << msg << std::endl;
        std::terminate();
    }
}
