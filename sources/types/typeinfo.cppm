export module atom.core:types.typeinfo;

import std;
import :types.typeinfo_impl;

namespace atom
{
    export template <typename in_value_t>
    class typeinfo
    {
        using this_t = typeinfo;

    public:
        using value_t = in_value_t;

        using add_const_t = typeinfo<std::add_const_t<value_t>>;
        using remove_const_t = typeinfo<std::remove_const_t<value_t>>;

        using add_volatile_t = typeinfo<std::add_volatile_t<value_t>>;
        using remove_volatile_t = typeinfo<std::remove_volatile_t<value_t>>;

        using unqualified_t = typeinfo<std::remove_cv_t<value_t>>;

        using add_lvalue_ref_t = typeinfo<std::add_lvalue_reference_t<value_t>>;
        using add_rvalue_ref_t = typeinfo<std::add_rvalue_reference_t<value_t>>;
        using remove_ref_t = typeinfo<std::remove_reference_t<value_t>>;

        using add_ptr_t = typeinfo<std::add_pointer_t<value_t>>;
        using remove_ptr_t = typeinfo<std::remove_pointer_t<value_t>>;

        using pure_t = typeinfo<std::remove_cvref_t<value_t>>;

        template <typename like_t>
        using unpure_like_t =
            typeinfo<typename typeinfo_impl::unpure_like_t<in_value_t, like_t>::value_t>;

        template <typename other_t>
        static constexpr bool is_same_as = std::is_same_v<value_t, other_t>;

        template <typename base_t>
        static constexpr bool is_derived_from = std::is_base_of_v<base_t, value_t>;

        template <typename base_t>
        static constexpr bool is_not_derived_from = not is_derived_from<base_t>;

        template <typename other_t>
        static constexpr bool is_same_or_derived_from =
            is_same_as<other_t> or is_derived_from<other_t>;

        static constexpr bool is_complete = typeinfo_impl::is_complete<value_t>::value;

        static constexpr bool is_void = std::is_void_v<value_t>;
        static constexpr bool is_not_void = not is_void;

        static constexpr bool is_lvalue_ref = std::is_lvalue_reference_v<value_t>;
        static constexpr bool is_not_lvalue_ref = not is_lvalue_ref;

        static constexpr bool is_rvalue_ref = std::is_rvalue_reference_v<value_t>;
        static constexpr bool is_not_rvalue_ref = not is_rvalue_ref;

        static constexpr bool is_ref = std::is_reference_v<value_t>;
        static constexpr bool is_not_ref = not is_ref;

        static constexpr bool is_const = std::is_const_v<value_t>;
        static constexpr bool is_not_const = not is_const;

        static constexpr bool is_volatile = std::is_volatile_v<value_t>;
        static constexpr bool is_not_volatile = not is_volatile;

        static constexpr bool is_qualified = is_const || is_volatile;
        static constexpr bool is_not_qualified = not is_qualified;

        static constexpr bool is_empty = std::is_empty_v<value_t>;
        static constexpr bool is_not_empty = not is_empty;

        static constexpr bool is_pure = not is_qualified and not is_ref;
        static constexpr bool is_not_pure = not is_pure;

        static constexpr bool is_enum = std::is_enum_v<value_t>;
        static constexpr bool is_not_enum = not is_enum;

        template <typename signature>
        static constexpr bool is_invokable =
            typeinfo_impl::template is_invokable<value_t, signature>::value;

        template <typename... args_t>
        static constexpr bool is_constructible_from = std::is_constructible_v<value_t, args_t...>;

        template <typename... args_t>
        static constexpr bool is_trivially_constructible_from =
            std::is_trivially_constructible_v<value_t, args_t...>;

        template <typename... args_t>
        static constexpr bool is_assignable_from = std::is_assignable_v<value_t, args_t...>;

        static constexpr bool is_swappable = std::is_swappable_v<value_t>;

        template <typename other_t>
        static constexpr bool is_convertible_to = std::is_convertible_v<value_t, other_t>;

        template <typename... args_t>
        static constexpr bool is_trivially_asignable_from =
            std::is_trivially_assignable_v<value_t, args_t...>;

        static constexpr bool is_default_constructible = std::is_default_constructible_v<value_t>;
        static constexpr bool is_not_default_constructible = not is_default_constructible;
        static constexpr bool is_trivially_default_constructible =
            std::is_trivially_default_constructible_v<value_t>;
        static constexpr bool is_not_trivially_default_constructible =
            not is_trivially_default_constructible;

        static constexpr bool is_copy_constructible = std::is_copy_constructible_v<value_t>;
        static constexpr bool is_not_copy_constructible = not is_copy_constructible;
        static constexpr bool is_trivially_copy_constructible =
            std::is_trivially_copy_constructible_v<value_t>;
        static constexpr bool is_not_trivially_copy_constructible =
            not is_trivially_copy_constructible;

        static constexpr bool is_copy_assignable = std::is_copy_assignable_v<value_t>;
        static constexpr bool is_not_copy_assignable = not is_copy_assignable;
        static constexpr bool is_trivially_copy_assignable =
            std::is_trivially_copy_assignable_v<value_t>;
        static constexpr bool is_not_trivially_copy_assignable = not is_trivially_copy_assignable;

        static constexpr bool is_copyable = is_copy_constructible and is_copy_assignable;
        static constexpr bool is_not_copyable = not is_copyable;
        static constexpr bool is_trivially_copyable =
            is_trivially_copy_constructible and is_trivially_copy_assignable;
        static constexpr bool is_not_trivially_copyable = not is_trivially_copyable;

        static constexpr bool is_move_constructible = std::is_move_constructible_v<value_t>;
        static constexpr bool is_not_move_constructible = not is_move_constructible;
        static constexpr bool is_trivially_move_constructible =
            std::is_trivially_move_constructible_v<value_t>;
        static constexpr bool is_not_trivially_move_constructible =
            not is_trivially_move_constructible;

        static constexpr bool is_move_assignable = std::is_move_assignable_v<value_t>;
        static constexpr bool is_not_move_assignable = not is_move_assignable;
        static constexpr bool is_trivially_move_assignable =
            std::is_trivially_move_assignable_v<value_t>;
        static constexpr bool is_not_trivially_move_assignable = not is_trivially_move_assignable;

        static constexpr bool is_moveable = is_move_constructible and is_move_assignable;
        static constexpr bool is_not_moveable = not is_moveable;
        static constexpr bool is_trivially_moveable =
            is_trivially_move_constructible and is_trivially_move_assignable;
        static constexpr bool is_not_trivially_moveable = not is_trivially_moveable;

        static constexpr bool is_destructible = std::is_destructible_v<value_t>;
        static constexpr bool is_not_destructible = not is_destructible;
        static constexpr bool is_trivially_destructible = std::is_trivially_destructible_v<value_t>;
        static constexpr bool is_not_trivially_destructible = not is_trivially_destructible;

        template <typename other_t>
        static constexpr bool is_equality_comparable_with = true;

        static constexpr bool is_equality_comparable = true;

        template <typename other_t>
        static constexpr bool is_comparable_with = true;

        static constexpr bool is_comparable = true;

        constexpr auto operator==(this const this_t& self, const this_t& other) -> bool
        {
            return true;
        }

        template <typename other_t>
        constexpr auto operator==(this const this_t& self, const other_t& other) -> bool
        {
            return false;
        }

        template <typename other_t>
        constexpr auto operator!=(this const this_t& self, const other_t& other) -> bool
        {
            return not self == other;
        }
    };
}
