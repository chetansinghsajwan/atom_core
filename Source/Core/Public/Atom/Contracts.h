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
        PostCondition
    };

    class ContractViolation
    {
    public:
        ContractType type;
        const char* msg;
        const char* expr;
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
                      << "at: " << violation.src.fileName
                      << ":" << violation.src.line << ":" << violation.src.column
                      << ": " << violation.src.funcName
                      << std::endl;

            std::terminate();
        }

    private:
        constexpr auto _toStr(ContractType type) -> std::string_view
        {
            switch (type)
            {
                case ContractType::PreCondition:  return "PreCondition";
                case ContractType::Assertion:     return "Assertion";
                case ContractType::PostCondition: return "PostCondition";
                default:                          return "[INVALID_VALUE]";
            }
        }
    };

    constexpr auto _ContractCheck(ContractType type, const char* expr,
        bool assert, const char* msg = "", SourceLineInfo src = SourceLineInfo::current())
    {
        if (assert) return;

        ContractViolation violation{ type, msg, expr, src };
        DefaultContractViolationHandler().handle(violation);
    }

    inline auto _Panic(const char* msg)
    {
        std::cerr << msg << std::endl;
        std::terminate();
    }
}

#define _ATOM_CONTRACT_CHECK(type, assertion, ...)                                         \
    ::Atom::_ContractCheck(type, #assertion, assertion, __VA_ARGS__)

#if ATOM_IS_CONFIG_DEBUG
#    define _ATOM_DEBUG_CONTRACT_CHECK(type, ...)                                          \
        _ATOM_CONTRACT_CHECK(type, __VA_ARGS__)
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
    _ATOM_DEBUG_CONTRACT_CHECK(::Atom::ContractType::PreCondition, __VA_ARGS__)

/// ------------------------------------------------------------------------------------------------
/// Represents assertion.
/// ------------------------------------------------------------------------------------------------
#define asserts(...) _ATOM_CONTRACT_CHECK(::Atom::ContractType::Assertion, __VA_ARGS__)

/// ------------------------------------------------------------------------------------------------
/// Represents debug assertion.
/// ------------------------------------------------------------------------------------------------
#define debug_asserts(...) _ATOM_DEBUG_CONTRACT_CHECK(::Atom::ContractType::Assertion, __VA_ARGS__)

/// ------------------------------------------------------------------------------------------------
/// Represents post condition.
/// ------------------------------------------------------------------------------------------------
// #define ensures(...) _ATOM_CONTRACT_CHECK(::Atom::ContractType::PostCondition, __VA_ARGS__)
#define ensures(...)

/// ------------------------------------------------------------------------------------------------
/// Represents debug post condition.
/// ------------------------------------------------------------------------------------------------
// #define debug_ensures(...)                                                                         \
//     _ATOM_DEBUG_CONTRACT_CHECK(::Atom::ContractType::PostCondition, __VA_ARGS__)
#define debug_ensures(...)

/// ------------------------------------------------------------------------------------------------
///
/// ------------------------------------------------------------------------------------------------
#define panic(...) ::Atom::_Panic(__VA_ARGS__);

#define fnret 0
#define static_expects(...) static_assert(__VA_ARGS__)
