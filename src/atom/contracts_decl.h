#pragma once
// #include <source_location>
// #include <string_view>

#include "atom/core/lang_extensions.h"

namespace atom
{
    enum class _contract_type
    {
        pre_condition,
        assertion,
        post_condition,
        debug_pre_condition,
        debug_assertion,
        debug_post_condition
    };

    inline auto _contract_check_impl(
        _contract_type type, std::source_location src, std::string_view msg) -> void;

    template <_contract_type type, typename... arg_types>
    constexpr auto _contract_check(std::source_location src, bool assert, arg_types&&... args) -> void
    {
        if constexpr (build_config::is_mode_debug() and type == _contract_type::debug_pre_condition
                      or type == _contract_type::debug_assertion
                      or type == _contract_type::debug_post_condition)
            return;

        if (assert)
            return;

        if (std::is_constant_evaluated())
            throw 0;

        std::string_view msg = "";
        _contract_check_impl(type, src, msg);
    }

    inline auto _panic(std::source_location src, std::string_view msg) -> void;
}

namespace atom::contracts
{
    /// ------------------------------------------------------------------------------------------------
    /// represents pre condition.
    /// ------------------------------------------------------------------------------------------------
    constexpr auto expects(bool assert, std::string_view msg = "",
        std::source_location _src = std::source_location::current())
    {
        _contract_check<_contract_type::pre_condition>(_src, assert, msg);
    }

    /// ------------------------------------------------------------------------------------------------
    /// represents debug pre condition.
    /// ------------------------------------------------------------------------------------------------
    constexpr auto debug_expects(bool assert, std::string_view msg = "",
        std::source_location _src = std::source_location::current())
    {
        _contract_check<_contract_type::debug_pre_condition>(_src, assert, msg);
    }

    /// ------------------------------------------------------------------------------------------------
    /// represents assertion.
    /// ------------------------------------------------------------------------------------------------
    constexpr auto asserts(bool assert, std::string_view msg = "",
        std::source_location _src = std::source_location::current())
    {
        _contract_check<_contract_type::assertion>(_src, assert, msg);
    }

    /// ------------------------------------------------------------------------------------------------
    /// represents debug assertion.
    /// ------------------------------------------------------------------------------------------------
    constexpr auto debug_asserts(bool assert, std::string_view msg = "",
        std::source_location _src = std::source_location::current())
    {
        _contract_check<_contract_type::debug_assertion>(_src, assert, msg);
    }

    /// ------------------------------------------------------------------------------------------------
    /// represents post condition.
    /// ------------------------------------------------------------------------------------------------
    constexpr auto ensures(bool assert, std::string_view msg = "",
        std::source_location _src = std::source_location::current())
    {
        _contract_check<_contract_type::post_condition>(_src, assert, msg);
    }

    /// ------------------------------------------------------------------------------------------------
    /// represents debug post condition.
    /// ------------------------------------------------------------------------------------------------
    constexpr auto debug_ensures(bool assert, std::string_view msg = "",
        std::source_location _src = std::source_location::current())
    {
        _contract_check<_contract_type::debug_post_condition>(_src, assert, msg);
    }
}

namespace atom::system
{
    /// ------------------------------------------------------------------------------------------------
    ///
    /// ------------------------------------------------------------------------------------------------
    template <typename... arg_types>
    constexpr auto panic(
        std::string_view msg = "", std::source_location _src = std::source_location::current())
    {
        _panic(_src, msg);
    }
}
