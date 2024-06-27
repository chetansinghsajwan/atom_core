export module atom.core:types.type_info;

import std;
import :types.type_info_impl;

namespace atom
{
    export using type_id = std::size_t;

    export template <typename in_value_type>
    class type_info
    {
        using this_type = type_info;
        using usize = std::size_t;

    public:
        using value_type = in_value_type;

        using add_const_type = type_info<std::add_const_t<value_type>>;
        using remove_const_type = type_info<std::remove_const_t<value_type>>;

        using add_volatile_type = type_info<std::add_volatile_t<value_type>>;
        using remove_volatile_type = type_info<std::remove_volatile_t<value_type>>;

        using unqualified_type = type_info<std::remove_cv_t<value_type>>;

        using add_lvalue_ref_type = type_info<std::add_lvalue_reference_t<value_type>>;
        using add_rvalue_ref_type = type_info<std::add_rvalue_reference_t<value_type>>;
        using remove_ref_type = type_info<std::remove_reference_t<value_type>>;

        using add_ptr_type = type_info<std::add_pointer_t<value_type>>;
        using remove_ptr_type = type_info<std::remove_pointer_t<value_type>>;

        using pure_type = type_info<std::remove_cvref_t<value_type>>;

        template <typename like_type>
        using unpure_like_type = type_info<
            typename type_info_impl::unpure_like_type<in_value_type, like_type>::value_type>;

        static consteval auto get_size() -> usize
        {
            return type_info_impl::get_sizeof<value_type>();
        }

        static consteval auto get_align() -> usize
        {
            return type_info_impl::get_alignof<value_type>();
        }

        template <typename other_type>
        static consteval auto is_same_as() -> bool
        {
            return std::is_same_v<value_type, other_type>;
        }

        template <typename base_type>
        static consteval auto is_derived_from() -> bool
        {
            return std::is_base_of_v<base_type, value_type>;
        }

        template <typename other_type>
        static consteval auto is_same_or_derived_from() -> bool
        {
            return is_same_as<other_type>() or is_derived_from<other_type>();
        }

        static consteval auto is_complete() -> bool
        {
            return type_info_impl::is_complete<value_type>::value;
        }

        static consteval auto is_void() -> bool
        {
            return std::is_void_v<value_type>;
        }

        static consteval auto is_lvalue_ref() -> bool
        {
            return std::is_lvalue_reference_v<value_type>;
        }

        static consteval auto is_rvalue_ref() -> bool
        {
            return std::is_rvalue_reference_v<value_type>;
        }

        static consteval auto is_ref() -> bool
        {
            return std::is_reference_v<value_type>;
        }

        static consteval auto is_const() -> bool
        {
            return std::is_const_v<value_type>;
        }

        static consteval auto is_volatile() -> bool
        {
            return std::is_volatile_v<value_type>;
        }

        static consteval auto is_qualified() -> bool
        {
            return is_const() || is_volatile();
        }

        static consteval auto is_empty() -> bool
        {
            return std::is_empty_v<value_type>;
        }

        static consteval auto is_pure() -> bool
        {
            return not is_qualified() and not is_ref();
        }

        static consteval auto is_enum() -> bool
        {
            return std::is_enum_v<value_type>;
        }

        template <typename signature>
        static consteval auto is_function() -> bool
        {
            return type_info_impl::template is_function<value_type, signature>::value;
        }

        template <typename... arg_types>
        static consteval auto is_constructible_from() -> bool
        {
            return std::is_constructible_v<value_type, arg_types...>;
        }

        template <typename... arg_types>
        static consteval auto is_trivially_constructible_from() -> bool
        {
            return std::is_trivially_constructible_v<value_type, arg_types...>;
        }

        template <typename... arg_types>
        static consteval auto is_assignable_from() -> bool
        {
            return std::is_assignable_v<value_type, arg_types...>;
        }

        static consteval auto is_swappable() -> bool
        {
            return std::is_swappable_v<value_type>;
        }

        template <typename other_type>
        static consteval auto is_convertible_to() -> bool
        {
            return std::is_convertible_v<value_type, other_type>;
        }

        template <typename... arg_types>
        static consteval auto is_trivially_asignable_from() -> bool
        {
            return std::is_trivially_assignable_v<value_type, arg_types...>;
        }

        static consteval auto is_default_constructible() -> bool
        {
            return std::is_default_constructible_v<value_type>;
        }

        static consteval auto is_trivially_default_constructible() -> bool
        {
            return std::is_trivially_default_constructible_v<value_type>;
        }

        static consteval auto is_copy_constructible() -> bool
        {
            return std::is_copy_constructible_v<value_type>;
        }

        static consteval auto is_trivially_copy_constructible() -> bool
        {
            return std::is_trivially_copy_constructible_v<value_type>;
        }

        static consteval auto is_copy_assignable() -> bool
        {
            return std::is_copy_assignable_v<value_type>;
        }

        static consteval auto is_trivially_copy_assignable() -> bool
        {
            return std::is_trivially_copy_assignable_v<value_type>;
        }

        static consteval auto is_copyable() -> bool
        {
            return is_copy_constructible() and is_copy_assignable();
        }

        static consteval auto is_trivially_copyable() -> bool
        {
            return is_trivially_copy_constructible() and is_trivially_copy_assignable();
        }

        static consteval auto is_move_constructible() -> bool
        {
            return std::is_move_constructible_v<value_type>;
        }

        static consteval auto is_trivially_move_constructible() -> bool
        {
            return std::is_trivially_move_constructible_v<value_type>;
        }

        static consteval auto is_move_assignable() -> bool
        {
            return std::is_move_assignable_v<value_type>;
        }

        static consteval auto is_trivially_move_assignable() -> bool
        {
            return std::is_trivially_move_assignable_v<value_type>;
        }

        static consteval auto is_moveable() -> bool
        {
            return is_move_constructible() and is_move_assignable();
        }

        static consteval auto is_trivially_moveable() -> bool
        {
            return is_trivially_move_constructible() and is_trivially_move_assignable();
        }

        static consteval auto is_destructible() -> bool
        {
            return std::is_destructible_v<value_type>;
        }

        static consteval auto is_trivially_destructible() -> bool
        {
            return std::is_trivially_destructible_v<value_type>;
        }

        template <typename other_type>
        static consteval auto is_equality_comparable_with() -> bool
        {
            return std::equality_comparable_with<value_type, other_type>;
        }

        static consteval auto is_equality_comparable() -> bool
        {
            return std::equality_comparable<value_type>;
        }

        template <typename other_type>
        static consteval auto is_comparable_with() -> bool
        {
            return true;
        }

        static consteval auto is_comparable() -> bool
        {
            return true;
        }

        constexpr auto operator==(const this_type& other) -> bool
        {
            return true;
        }

        template <typename other_type>
        constexpr auto operator==(const other_type& other) -> bool
        {
            return false;
        }

        template <typename other_type>
        constexpr auto operator!=(const other_type& other) -> bool
        {
            return not *this == other;
        }

        static auto get_id() -> type_id
        {
            return typeid(value_type).hash_code();
        }
    };
}
