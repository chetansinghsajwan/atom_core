#pragma once
#include <type_traits>

namespace atom
{
    template <typename value_t>
    class typeinfo
    {
    public:
        using pure_t = std::remove_cvref_t<value_t>;

        using with_const_t = std::add_const_t<value_t>;
        using without_const_t = std::remove_const_t<value_t>;
        using with_ref_t = std::add_const_t<value_t>;
        using without_ref_t = std::remove_const_t<value_t>;

        using remove_cvref_t = std::remove_cvref_t<value_t>;
        using remove_quailfiers_ref_t = std::remove_cvref_t<value_t>;
        using unqualified_t = std::remove_cv_t<value_t>;

        template <typename invokable_t>
        using get_return_t = std::invoke_result_t<invokable_t>;

        template <typename other_t>
        static constexpr bool is_same_as = std::is_same_v<value_t, other_t>;

        static constexpr bool is_void = std::is_void_v<value_t>;
        static constexpr bool is_ref = std::is_reference_v<value_t>;
        static constexpr bool is_lref = std::is_lvalue_reference_v<value_t>;
        static constexpr bool is_rref = std::is_rvalue_reference_v<value_t>;
        static constexpr bool is_const = std::is_const_v<value_t>;
        static constexpr bool is_volatile = std::is_volatile_v<value_t>;
        static constexpr bool is_qualified = is_const || is_volatile;
        static constexpr bool is_empty = std::is_empty_v<value_t>;
        static constexpr bool is_pure = not is_qualified and not is_ref;
        static constexpr bool is_enum = std::is_enum_v<value_t>;
        static constexpr bool is_int = std::is_integral_v<value_t>;

        template <typename base_t>
        static constexpr bool is_derived_from = std::is_base_of_v<base_t, value_t>;

        template <typename... args_t>
        static constexpr bool is_constructible_from = std::is_constructible_v<value_t, args_t...>;
        static constexpr bool is_default_constructible = std::is_default_constructible_v<value_t>;
        static constexpr bool is_copy_constructible = std::is_copy_constructible_v<value_t>;
        static constexpr bool is_move_constructible = std::is_copy_constructible_v<value_t>;
        static constexpr bool is_copy_assignable = std::is_copy_assignable_v<value_t>;
        static constexpr bool is_move_assignable = std::is_move_assignable_v<value_t>;
        static constexpr bool is_destructible = std::is_destructible_v<value_t>;

        template <typename... args_t>
        static constexpr bool is_trivially_constructible_from = std::is_trivially_constructible_v<value_t, args_t...>;
        static constexpr bool is_trivially_default_constructible = std::is_trivially_default_constructible_v<value_t>;
        static constexpr bool is_trivially_copy_constructible = std::is_trivially_copy_constructible_v<value_t>;
        static constexpr bool is_trivially_move_constructible = std::is_trivially_copy_constructible_v<value_t>;
        static constexpr bool is_trivially_copy_assignable = std::is_trivially_copy_assignable_v<value_t>;
        static constexpr bool is_trivially_move_assignable = std::is_trivially_move_assignable_v<value_t>;
        static constexpr bool is_trivially_destructible = std::is_trivially_destructible_v<value_t>;
    };

    class typeutils
    {
    public:
        class _empty
        {};

        template <bool condition, typename true_t, typename false_t>
        using conditional_t = std::conditional_t<condition, true_t, false_t>;

        template <bool condition, typename true_t, typename false_t = _empty>
        using conditional_field_t = conditional_t<condition, true_t, _empty>;

        template <bool requirements, typename value_t = void>
        using enable_if_t = std::enable_if_t<requirements, value_t>;

        template <typename value_t>
        using identity_t = std::type_identity_t<value_t>;
    };
}
