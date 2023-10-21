#pragma once
#include "Atom/Core.h"
#include "Atom/Core/SourceLineInfo.h"
#include "Contracts.decl.h"

#include <iostream>

namespace Atom
{
    enum class ContractType
    {
        PreCondition,
        Assertion,
        PostCondition,
        DebugPreCondition,
        DebugAssertion,
        DebugPostCondition
    };

    class ContractViolation
    {
    public:
        ContractType type;
        std::string_view msg;
        std::string_view expr;
        SourceLineInfo src;
    };

    class ContractViolationHandler
    {
    public:
        virtual auto handle(const ContractViolation& violation) -> void = 0;
    };

    class DefaultContractViolationHandler final: public ContractViolationHandler
    {
    public:
        virtual auto handle(const ContractViolation& violation) -> void override
        {
            std::cout << "Contracts " << _toStr(violation.type) << " Violation: "
                      << "'" << violation.expr << "'\n\t"
                      << "with msg: " << violation.msg << "'\n\t"
                      << "at: " << violation.src.fileName << ":" << violation.src.line.val() << ":"
                      << violation.src.column.val() << ": " << violation.src.funcName << std::endl;

            std::terminate();
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

    constexpr auto _ContractCheck(_ContractType type, std::string_view expr, bool assert,
        std::string_view msg, std::source_location src) -> void
    {
        if (assert)
            return;

        SourceLineInfo srcInfo
        {
            .line = src.line(),
            .column = src.column(),
            .funcName = src.function_name(),
            .fileName = src.file_name()
        };

        ContractViolation violation{ ContractType(type), msg, expr, srcInfo };
        DefaultContractViolationHandler().handle(violation);
    }

    inline auto _Panic(std::string_view msg)
    {
        std::cerr << msg << std::endl;
        std::terminate();
    }
}