#pragma once
#include <type_traits>

namespace atom::typeinfo
{
    class _empty
    {};

    template <bool condition, typename true_t, typename false_t>
    using conditional_t = std::conditional_t<condition, true_t, false_t>;

    template <bool condition, typename true_t, typename false_t = _empty>
    using conditional_field_t = conditional_t<condition, true_t, _empty>;

    template <typename value_t>
    using remove_const_t = std::remove_const_t<value_t>;

    template <typename value_t>
    using remove_cvref_t = std::remove_cvref_t<value_t>;

    template <typename value_t>
    using remove_quailfiers_ref_t = std::remove_cvref_t<value_t>;

    template <typename value_t>
    using unqualified_t = std::remove_cv_t<value_t>;

    template <typename value_t>
    using get_pure = remove_quailfiers_ref_t<value_t>;

    template <typename invokable_t>
    using get_return_t = std::invoke_result_t<invokable_t>;

    template <bool requirements, typename value_t = void>
    using enable_if_t = std::enable_if_t<requirements, value_t>;

    template <typename value_t>
    using identity_t = std::type_identity_t<value_t>;

    template <typename enum_t>
    constexpr bool is_enum = std::is_enum_v<enum_t>;

    template <typename int_t>
    constexpr bool is_int = std::is_integral_v<int_t>;

    template <typename derived_t, typename base_t>
    constexpr bool is_derived_from = std::is_base_of_v<base_t, derived_t>;

    template <typename value_t, typename... arg_ts>
    constexpr bool is_constructible_from = std::is_constructible_v<value_t, arg_ts...>;

    template <typename value_t>
    constexpr bool is_default_constructible = std::is_default_constructible_v<value_t>;

    template <typename value_t>
    constexpr bool is_copy_constructible = std::is_copy_constructible_v<value_t>;

    template <typename value_t>
    constexpr bool is_move_constructible = std::is_copy_constructible_v<value_t>;

    template <typename value_t>
    constexpr bool is_copy_assignable = std::is_copy_assignable_v<value_t>;

    template <typename value_t>
    constexpr bool is_move_assignable = std::is_move_assignable_v<value_t>;

    template <typename type0, typename type1>
    constexpr bool is_same = std::is_same_v<type0, type1>;

    template <typename value_t>
    constexpr bool is_void = std::is_void_v<value_t>;

    template <typename value_t>
    constexpr bool is_ref = std::is_reference_v<value_t>;

    template <typename value_t>
    constexpr bool is_rvalue_ref = std::is_rvalue_reference_v<value_t>;

    template <typename value_t>
    constexpr bool is_const = std::is_const_v<value_t>;

    template <typename value_t>
    constexpr bool is_volatile = std::is_volatile_v<value_t>;

    template <typename value_t>
    constexpr bool is_qualified = is_const<value_t> || is_volatile<value_t>;

    template <typename value_t>
    constexpr bool is_pure = not is_qualified<value_t> and not is_ref<value_t>;

    template <typename value_t>
    constexpr bool is_empty = std::is_empty_v<value_t>;
}
