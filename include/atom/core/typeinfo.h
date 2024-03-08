#pragma once
#include <type_traits>

namespace atom::typeinfo
{
    class _empty
    {};

    template <bool condition, typename true_type, typename false_type>
    using conditional_type = std::conditional_t<condition, true_type, false_type>;

    template <bool condition, typename true_type, typename false_type = _empty>
    using conditional_field_type = conditional_type<condition, true_type, _empty>;

    template <typename value_type>
    using remove_const_type = std::remove_const_t<value_type>;

    template <typename value_type>
    using remove_cvref_type = std::remove_cvref_t<value_type>;

    template <typename value_type>
    using remove_quailfiers_ref_type = std::remove_cvref_t<value_type>;

    template <typename value_type>
    using unqualified_type = std::remove_cv_t<value_type>;

    template <typename value_type>
    using get_pure = remove_quailfiers_ref_type<value_type>;

    template <typename invokable_type>
    using get_return_type = std::result_of_t<invokable_type>;

    template <bool requirements, typename value_type = void>
    using enable_if_type = std::enable_if_t<requirements, value_type>;

    template <typename base_type, typename derived_type>
    constexpr bool is_base_of = std::is_base_of_v<base_type, std::decay_t<derived_type>>;

    template <typename derived_type, typename base_type>
    constexpr bool is_derived_from = std::is_base_of_v<base_type, derived_type>;

    template <typename base_type, typename derived_type>
    constexpr bool is_not_base_of = !is_base_of<base_type, derived_type>;

    template <typename value_type>
    constexpr bool is_copy_constructible = std::is_copy_constructible_v<value_type>;

    template <typename value_type>
    constexpr bool is_move_constructible = std::is_copy_constructible_v<value_type>;

    template <typename value_type>
    constexpr bool is_copy_assignable = std::is_copy_assignable_v<value_type>;

    template <typename value_type>
    constexpr bool is_move_assignable = std::is_move_assignable_v<value_type>;

    template <typename type0, typename type1>
    constexpr bool is_same = std::is_same_v<type0, type1>;

    template <typename type0, typename type1>
    constexpr bool is_not_same = !is_same<type0, type1>;

    template <typename value_type>
    constexpr bool is_void = std::is_void_v<value_type>;

    template <typename value_type>
    constexpr bool is_ref = std::is_reference_v<value_type>;

    template <typename value_type>
    constexpr bool is_rvalue_ref = std::is_rvalue_reference_v<value_type>;

    template <typename value_type>
    constexpr bool is_const = std::is_const_v<value_type>;

    template <typename value_type>
    constexpr bool is_volatile = std::is_volatile_v<value_type>;

    template <typename value_type>
    constexpr bool is_qualified = is_const<value_type> || is_volatile<value_type>;

    template <typename value_type>
    constexpr bool is_pure = not is_qualified<value_type> and not is_ref<value_type>;

    template <typename value_type>
    constexpr bool is_empty = std::is_empty_v<value_type>;
}
