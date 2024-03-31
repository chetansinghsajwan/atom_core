#pragma once
#include "atom/core/_std.h"
#include "atom/core/core/core.h"

namespace atom
{
    template <typename value_t>
    constexpr auto move(value_t&& val) -> typename std::remove_reference_t<value_t>&&
    {
        return static_cast<typename std::remove_reference_t<value_t>&&>(val);
    }

    template <typename value_t>
    constexpr auto forward(typename std::remove_reference_t<value_t>& val) -> value_t&&
    {
        return static_cast<value_t&&>(val);
    }

    template <typename value_t>
    constexpr auto forward(typename std::remove_reference_t<value_t>&& val) -> value_t&&
    {
        static_assert(
            !std::is_lvalue_reference_v<value_t>, "can not forward an rvalue as an lvalue.");

        return static_cast<value_t&&>(val);
    }

#define ATOM_FORWARD(...) static_cast<decltype(__VA_ARGS__)&&>(__VA_ARGS__)
#define ATOM_MOVE(...) static_cast<std::remove_reference_t<decltype(__VA_ARGS__)>&&>(__VA_ARGS__)
}
