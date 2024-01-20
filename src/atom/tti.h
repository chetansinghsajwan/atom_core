#pragma once
// #include <type_traits>

#include "atom/core/lang_extensions.h"

namespace atom::tti
{
    class _empty
    {};

    template <bool condition, typename ttrue, typename tfalse>
    using conditional_type = std::conditional_t<condition, ttrue, tfalse>;

    template <bool condition, typename true_type, typename false_type = _empty>
    using conditional_field_type = conditional_type<condition, true_type, _empty>;

    template <typename base_type, typename derived_type>
    constexpr bool is_base_of = std::is_base_of_v<base_type, std::decay_t<derived_type>>;

    template <typename base_type, typename derived_type>
    constexpr bool is_not_base_of = !is_base_of<base_type, derived_type>;

    template <bool requirements, typename type = void>
    using enable_if_type = std::enable_if_t<requirements, type>;

    template <typename type>
    constexpr bool is_copy_constructible = std::is_copy_constructible_v<type>;

    template <typename type>
    constexpr bool is_move_constructible = std::is_copy_constructible_v<type>;

    template <typename type>
    constexpr bool is_copy_assignable = std::is_copy_assignable_v<type>;

    template <typename type>
    constexpr bool is_move_assignable = std::is_move_assignable_v<type>;

    template <typename type0, typename type1>
    constexpr bool is_same = std::is_same_v<type0, type1>;

    template <typename type0, typename type1>
    constexpr bool is_not_same = !is_same<type0, type1>;

    template <typename type>
    using remove_const_type = std::remove_const_t<type>;

    template <typename type>
    using remove_cvref_type = std::remove_cvref_t<type>;

    template <typename type>
    using remove_quailfiers_ref_type = std::remove_cvref_t<type>;

    template <typename type>
    using unqualified_type = std::remove_cv_t<type>;

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
    using pure_type = remove_quailfiers_ref_type<type>;

    template <typename type>
    constexpr bool is_pure = not is_qualified<type> and not is_ref<type>;

    template <typename type>
    constexpr bool is_empty = std::is_empty_v<type>;
}
