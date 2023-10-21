#pragma once
#include <string_view>
#include <source_location>

namespace Atom
{
    enum class _ContractType
    {
        PreCondition,
        Assertion,
        PostCondition,
        DebugPreCondition,
        DebugAssertion,
        DebugPostCondition
    };

    consteval auto _FindContractExpr(std::string_view str) -> std::string_view
    {
        size_t i = str.find(',');
        return { str.data(), i };
    }

    constexpr auto _ContractCheck(_ContractType type, std::string_view expr, bool assert,
        std::string_view msg = "", std::source_location src = std::source_location::current()) -> void;

    inline auto _Panic(std::string_view msg);
}

/// ------------------------------------------------------------------------------------------------
///
/// ------------------------------------------------------------------------------------------------
#define _ATOM_CONTRACT_CHECK(type, ...)                                                            \
    Atom::_ContractCheck(type, Atom::_FindContractExpr(#__VA_ARGS__), __VA_ARGS__)

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
#define expects(...) _ATOM_CONTRACT_CHECK(Atom::_ContractType::PreCondition, __VA_ARGS__)

/// ------------------------------------------------------------------------------------------------
/// Represents debug pre condition.
/// ------------------------------------------------------------------------------------------------
#define debug_expects(...)                                                                         \
    _ATOM_DEBUG_CONTRACT_CHECK(Atom::_ContractType::DebugPreCondition, __VA_ARGS__)

/// ------------------------------------------------------------------------------------------------
/// Represents assertion.
/// ------------------------------------------------------------------------------------------------
#define asserts(...) _ATOM_CONTRACT_CHECK(Atom::_ContractType::Assertion, __VA_ARGS__)

/// ------------------------------------------------------------------------------------------------
/// Represents debug assertion.
/// ------------------------------------------------------------------------------------------------
#define debug_asserts(...)                                                                         \
    _ATOM_DEBUG_CONTRACT_CHECK(Atom::_ContractType::DebugAssertion, __VA_ARGS__)

/// ------------------------------------------------------------------------------------------------
/// Represents post condition.
/// ------------------------------------------------------------------------------------------------
// #define ensures(...) _ATOM_CONTRACT_CHECK(Atom::_ContractType::PostCondition, __VA_ARGS__)
#define ensures(...)

/// ------------------------------------------------------------------------------------------------
/// Represents debug post condition.
/// ------------------------------------------------------------------------------------------------
// #define debug_ensures(...)                                                                         \
//     _ATOM_DEBUG_CONTRACT_CHECK(Atom::_ContractType::DebugPostCondition, __VA_ARGS__)
#define debug_ensures(...)

/// ------------------------------------------------------------------------------------------------
///
/// ------------------------------------------------------------------------------------------------
#define panic(...) Atom::_Panic(__VA_ARGS__);

#define fnret 0
