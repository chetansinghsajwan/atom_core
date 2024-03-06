#pragma once
#include "atom/core/core/build_config.h"
#include "atom/core/core/source_location.h"

namespace atom
{
    enum class contract_type
    {
        expects,
        asserts,
        ensures,
        debug_expects,
        debug_asserts,
        debug_ensures
    };

    inline auto _handle_contract_violation_impl(contract_type type, std::string_view expr,
        source_location source, std::string_view msg) -> void;

    inline auto _panic(source_location source, std::string_view msg) -> void;

    template <typename... arg_types>
    constexpr auto _handle_contract_violation(contract_type type, std::string_view expr,
        source_location source, arg_types&&... args) -> void
    {
        if (std::is_constant_evaluated())
            throw 0;

        std::string_view msg(std::forward<arg_types>(args)...);
        _handle_contract_violation_impl(type, expr, source, msg);
    }
}

#define VA_ARGS(...) , ##__VA_ARGS__

#define _ATOM_CONTRACT_CHECK(CONTRACT_TYPE, ASSERTION, ...)                                        \
    if (not(ASSERTION))                                                                            \
    atom::_handle_contract_violation(atom::contract_type::CONTRACT_TYPE, #ASSERTION,               \
        atom::source_location::current() VA_ARGS(__VA_ARGS__))

#if defined(ATOM_MODE_DEBUG)
#    define _ATOM_CONTRACT_CHECK_DEBUG(...) _ATOM_CONTRACT_CHECK(__VA_ARGS__)
#else
#    define _ATOM_CONTRACT_CHECK_DEBUG(...)
#endif

#define ATOM_EXPECTS(...) _ATOM_CONTRACT_CHECK(expects, __VA_ARGS__)
#define ATOM_ASSERTS(...) _ATOM_CONTRACT_CHECK(asserts, __VA_ARGS__)
#define ATOM_ENSURES(...) _ATOM_CONTRACT_CHECK(ensures, __VA_ARGS__)
#define ATOM_DEBUG_EXPECTS(...) _ATOM_CONTRACT_CHECK_DEBUG(debug_expects, __VA_ARGS__)
#define ATOM_DEBUG_ASSERTS(...) _ATOM_CONTRACT_CHECK_DEBUG(debug_asserts, __VA_ARGS__)
#define ATOM_DEBUG_ENSURES(...) _ATOM_CONTRACT_CHECK_DEBUG(debug_ensures, __VA_ARGS__)
#define ATOM_STATIC_ASSERTS(...) static_assert(__VA_ARGS__)
#define ATOM_PANIC(...) atom::_panic(atom::source_location::current() VA_ARGS(__VA_ARGS__))
