#pragma once
// #include <type_traits>

#include "atom/core/build_config.h"
#include "atom/core/lang_extensions.h"

namespace atom::tti
{
    class _empty
    {};

    template <bool condition, typename ttrue, typename tfalse>
    using tconditional = std::conditional_t<condition, ttrue, tfalse>;

    template <bool condition, typename type>
    using tconditional_field = tconditional<condition, type, _empty>;

    template <typename tbase, typename tderived>
    constexpr bool is_base_of = std::is_base_of_v<tbase, std::decay_t<tderived>>;

    template <typename tbase, typename tderived>
    constexpr bool is_not_base_of = !is_base_of<tbase, tderived>;

    template <bool requirements, typename type = void>
    using tenable_if = std::enable_if_t<requirements, type>;

    template <typename type>
    constexpr bool is_copy_constructible = std::is_copy_constructible_v<type>;

    template <typename type>
    constexpr bool is_move_constructible = std::is_copy_constructible_v<type>;

    template <typename type>
    constexpr bool is_copy_assignable = std::is_copy_assignable_v<type>;

    template <typename type>
    constexpr bool is_move_assignable = std::is_move_assignable_v<type>;

    template <typename t0, typename t1>
    constexpr bool is_same = std::is_same_v<t0, t1>;

    template <typename t0, typename t1>
    constexpr bool is_not_same = !is_same<t0, t1>;

    template <typename type>
    using tremove_const = std::remove_const_t<type>;

    template <typename type>
    using tremove_cvref = std::remove_cvref_t<type>;

    template <typename type>
    using tremove_quailfiers_ref = std::remove_cvref_t<type>;

    template <typename type>
    using tunqualified = std::remove_cv_t<type>;

    template <typename type>
    constexpr bool is_void = std::is_void_v<type>;

    template <typename type>
    constexpr bool is_ref = std::is_reference_v<type>;

    template <typename type>
    constexpr bool is_rvalue_ref = std::is_rvalue_reference_v<type>;

    template <typename type>
    constexpr bool is_const = std::is_const_v<type>;

    template <typename type>
    constexpr bool is_volatile = std::is_volatile_v<type>;

    template <typename type>
    constexpr bool is_qualified = is_const<type> || is_volatile<type>;

    template <typename type>
    using tpure = tremove_quailfiers_ref<type>;

    template <typename type>
    constexpr bool is_pure = not is_qualified<type> and not is_ref<type>;

    template <typename type>
    constexpr bool is_empty = std::is_empty_v<type>;
}
