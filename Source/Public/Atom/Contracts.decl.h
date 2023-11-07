#pragma once
#include <source_location>
#include <string_view>

#include "Atom/Core/BuildConfig.h"
#include "Atom/Core/LangExtensions.h"

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

    inline auto _ContractCheckImpl(
        _ContractType type, std::source_location src, std::string_view msg) -> void;

    template <_ContractType type, typename... TArgs>
    constexpr auto _ContractCheck(std::source_location src, bool assert, TArgs&&... args) -> void
    {
        if constexpr (BuildConfig::IsModeDebug() and type == _ContractType::DebugPreCondition
                      or type == _ContractType::DebugAssertion
                      or type == _ContractType::DebugPostCondition)
            return;

        if (assert)
            return;

        if (std::is_constant_evaluated())
            throw 0;

        std::string_view msg = "";
        _ContractCheckImpl(type, src, msg);
    }

    inline auto _Panic(std::source_location src, std::string_view msg) -> void;
}

namespace Atom
{
    /// ------------------------------------------------------------------------------------------------
    /// Represents pre condition.
    /// ------------------------------------------------------------------------------------------------
    constexpr auto expects(bool assert, std::string_view msg = "",
        std::source_location _src = std::source_location::current())
    {
        _ContractCheck<_ContractType::PreCondition>(_src, assert, msg);
    }

    /// ------------------------------------------------------------------------------------------------
    /// Represents debug pre condition.
    /// ------------------------------------------------------------------------------------------------
    constexpr auto debug_expects(bool assert, std::string_view msg = "",
        std::source_location _src = std::source_location::current())
    {
        _ContractCheck<_ContractType::DebugPreCondition>(_src, assert, msg);
    }

    /// ------------------------------------------------------------------------------------------------
    /// Represents assertion.
    /// ------------------------------------------------------------------------------------------------
    constexpr auto asserts(bool assert, std::string_view msg = "",
        std::source_location _src = std::source_location::current())
    {
        _ContractCheck<_ContractType::Assertion>(_src, assert, msg);
    }

    /// ------------------------------------------------------------------------------------------------
    /// Represents debug assertion.
    /// ------------------------------------------------------------------------------------------------
    constexpr auto debug_asserts(bool assert, std::string_view msg = "",
        std::source_location _src = std::source_location::current())
    {
        _ContractCheck<_ContractType::DebugAssertion>(_src, assert, msg);
    }

    /// ------------------------------------------------------------------------------------------------
    /// Represents post condition.
    /// ------------------------------------------------------------------------------------------------
    constexpr auto ensures(bool assert, std::string_view msg = "",
        std::source_location _src = std::source_location::current())
    {
        _ContractCheck<_ContractType::PostCondition>(_src, assert, msg);
    }

    /// ------------------------------------------------------------------------------------------------
    /// Represents debug post condition.
    /// ------------------------------------------------------------------------------------------------
    constexpr auto debug_ensures(bool assert, std::string_view msg = "",
        std::source_location _src = std::source_location::current())
    {
        _ContractCheck<_ContractType::DebugPostCondition>(_src, assert, msg);
    }

    /// ------------------------------------------------------------------------------------------------
    ///
    /// ------------------------------------------------------------------------------------------------
    template <typename... TArgs>
    constexpr auto panic(
        std::string_view msg = "", std::source_location _src = std::source_location::current())
    {
        _Panic(_src, msg);
    }
}
