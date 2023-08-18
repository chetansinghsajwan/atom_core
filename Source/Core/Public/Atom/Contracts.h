#pragma once
#include "Atom/Core.h"

#include <iostream>

namespace Atom
{
    enum class ContractType
    {
        PreCondition,
        Assertion,
        PostCondition
    };

    constexpr auto _ContractCheck(ContractType contractType, bool assert, const char* msg = "") {}

    inline auto _Panic(const char* msg)
    {
        std::cerr << msg << std::endl;
        std::terminate();
    }
}

#define _ATOM_CONTRACT_CHECK(contractType, ...) ::Atom::_ContractCheck(contractType, __VA_ARGS__)

#if ATOM_IS_CONFIG_DEBUG
#    define _ATOM_DEBUG_CONTRACT_CHECK(contractType, ...)                                          \
        _ATOM_CONTRACT_CHECK(contractType, __VA_ARGS__)
#else
#    define _ATOM_DEBUG_CONTRACT_CHECK(contractType, ...)
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
