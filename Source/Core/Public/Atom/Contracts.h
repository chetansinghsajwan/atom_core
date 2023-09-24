#pragma once
#include "Atom/Core.h"
#include "Atom/Core/SourceLineInfo.h"

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

    consteval StdStrView _FindContractExpr(StdStrView str)
    {
        usize i = str.find(',');
        return { str.data(), i.val() };
    }

    class ContractViolation
    {
    public:
        ContractType type;
        StdStrView msg;
        StdStrView expr;
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
        constexpr auto _toStr(ContractType type) -> StdStrView
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

    constexpr auto _ContractCheck(ContractType type, StdStrView expr, bool assert,
        StdStrView msg = "", SourceLineInfo src = SourceLineInfo::current())
    {
        if (assert)
            return;

        ContractViolation violation{ type, msg, expr, src };
        DefaultContractViolationHandler().handle(violation);
    }

    inline auto _Panic(StdStrView msg)
    {
        std::cerr << msg << std::endl;
        std::terminate();
    }
}

/// ------------------------------------------------------------------------------------------------
///
/// ------------------------------------------------------------------------------------------------
#define _ATOM_CONTRACT_CHECK(type, ...)                                                            \
    ::Atom::_ContractCheck(type, ::Atom::_FindContractExpr(#__VA_ARGS__), __VA_ARGS__)

/// ------------------------------------------------------------------------------------------------
///
/// ------------------------------------------------------------------------------------------------
#if ATOM_IS_CONFIG_DEBUG
#    define _ATOM_DEBUG_CONTRACT_CHECK(type, ...) _ATOM_CONTRACT_CHECK(type, __VA_ARGS__)
#else
#    define _ATOM_DEBUG_CONTRACT_CHECK(type, ...)
#endif

/// ------------------------------------------------------------------------------------------------
/// Represents pre condition.
/// ------------------------------------------------------------------------------------------------
#define expects(...) _ATOM_CONTRACT_CHECK(::Atom::ContractType::PreCondition, __VA_ARGS__)

/// ------------------------------------------------------------------------------------------------
/// Represents debug pre condition.
/// ------------------------------------------------------------------------------------------------
#define debug_expects(...)                                                                         \
    _ATOM_DEBUG_CONTRACT_CHECK(::Atom::ContractType::DebugPreCondition, __VA_ARGS__)

/// ------------------------------------------------------------------------------------------------
/// Represents assertion.
/// ------------------------------------------------------------------------------------------------
#define asserts(...) _ATOM_CONTRACT_CHECK(::Atom::ContractType::Assertion, __VA_ARGS__)

/// ------------------------------------------------------------------------------------------------
/// Represents debug assertion.
/// ------------------------------------------------------------------------------------------------
#define debug_asserts(...)                                                                         \
    _ATOM_DEBUG_CONTRACT_CHECK(::Atom::ContractType::DebugAssertion, __VA_ARGS__)

/// ------------------------------------------------------------------------------------------------
/// Represents post condition.
/// ------------------------------------------------------------------------------------------------
// #define ensures(...) _ATOM_CONTRACT_CHECK(::Atom::ContractType::PostCondition, __VA_ARGS__)
#define ensures(...)

/// ------------------------------------------------------------------------------------------------
/// Represents debug post condition.
/// ------------------------------------------------------------------------------------------------
// #define debug_ensures(...)                                                                         \
//     _ATOM_DEBUG_CONTRACT_CHECK(::Atom::ContractType::DebugPostCondition, __VA_ARGS__)
#define debug_ensures(...)

/// ------------------------------------------------------------------------------------------------
///
/// ------------------------------------------------------------------------------------------------
#define panic(...) ::Atom::_Panic(__VA_ARGS__);

#define fnret 0
