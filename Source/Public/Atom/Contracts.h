#pragma once
#include "Atom/Core.h"
#include "Atom/Core/SourceLineInfo.h"
#include "Contracts.decl.h"

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
        std::string_view expr;
        SourceLineInfo src;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    class ContractViolationException
    {
    public:
        ContractViolation violation;
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
            if constexpr (ATOM_IS_CONFIG_DEBUG)
            {
                throw ContractViolationException{ violation };
            }
            else
            {
                std::cout << "Contracts " << _toStr(violation.type) << " Violation: "
                        << "'" << violation.expr << "'\n\t"
                        << "with msg: " << violation.msg << "'\n\t"
                        << "at: " << violation.src.fileName << ":" << violation.src.line.val() << ":"
                        << violation.src.column.val() << ": " << violation.src.funcName << std::endl;

                std::terminate();
            }
        }

    private:
        constexpr auto _toStr(ContractType type) -> std::string_view
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
    constexpr auto _ContractCheck(_ContractType type, std::string_view expr, bool assert,
        std::string_view msg, std::source_location src) -> void
    {
        if (assert)
            return;

        if (std::is_constant_evaluated())
            throw 0;

        SourceLineInfo srcInfo{ .line = src.line(),
            .column = src.column(),
            .funcName = src.function_name(),
            .fileName = src.file_name() };

        ContractViolation violation{ ContractType(type), msg, expr, srcInfo };
        ContractViolationHandlerManager::GetHandler().handle(violation);
    }

    inline auto _Panic(std::string_view msg)
    {
        std::cerr << msg << std::endl;
        std::terminate();
    }
}
