export module atom_core:types.type_info_impl;

import std;

namespace atom
{
    namespace type_info_impl
    {
        using usize = std::size_t;

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename value_type>
        consteval auto get_sizeof() -> usize
        {
            if constexpr (std::is_void_v<value_type>)
            {
                return 0;
            }
            else
            {
                return sizeof(value_type);
            }
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename value_type>
        consteval auto get_alignof() -> usize
        {
            if constexpr (std::is_void_v<value_type>)
            {
                return 0;
            }
            else
            {
                return alignof(value_type);
            }
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------

        template <class T, class = void>
        struct is_complete
        {
            static constexpr bool value = false;
        };

        template <class T>
        struct is_complete<T, decltype(void(sizeof(T)))>
        {
            static constexpr bool value = true;
        };

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------

        template <typename in_value_type, typename other_type>
        struct unpure_like_type
        {
            using value_type = std::remove_cvref_t<in_value_type>;
        };

        template <typename in_value_type, typename other_type>
        struct unpure_like_type<in_value_type, const other_type>
        {
            using value_type = const std::remove_cvref_t<in_value_type>;
        };

        template <typename in_value_type, typename other_type>
        struct unpure_like_type<in_value_type, volatile other_type>
        {
            using value_type = volatile std::remove_cvref_t<in_value_type>;
        };

        template <typename in_value_type, typename other_type>
        struct unpure_like_type<in_value_type, const volatile other_type>
        {
            using value_type = const volatile std::remove_cvref_t<in_value_type>;
        };

        template <typename in_value_type, typename other_type>
        struct unpure_like_type<in_value_type, other_type&>
        {
            using value_type = std::remove_cvref_t<in_value_type>&;
        };

        template <typename in_value_type, typename other_type>
        struct unpure_like_type<in_value_type, other_type&&>
        {
            using value_type = std::remove_cvref_t<in_value_type>&&;
        };

        template <typename in_value_type, typename other_type>
        struct unpure_like_type<in_value_type, const other_type&>
        {
            using value_type = const std::remove_cvref_t<in_value_type>&;
        };

        template <typename in_value_type, typename other_type>
        struct unpure_like_type<in_value_type, const other_type&&>
        {
            using value_type = const std::remove_cvref_t<in_value_type>&&;
        };

        template <typename in_value_type, typename other_type>
        struct unpure_like_type<in_value_type, volatile other_type&>
        {
            using value_type = volatile std::remove_cvref_t<in_value_type>&;
        };

        template <typename in_value_type, typename other_type>
        struct unpure_like_type<in_value_type, volatile other_type&&>
        {
            using value_type = volatile std::remove_cvref_t<in_value_type>&&;
        };

        template <typename in_value_type, typename other_type>
        struct unpure_like_type<in_value_type, const volatile other_type&>
        {
            using value_type = const volatile std::remove_cvref_t<in_value_type>&;
        };

        template <typename in_value_type, typename other_type>
        struct unpure_like_type<in_value_type, const volatile other_type&&>
        {
            using value_type = const volatile std::remove_cvref_t<in_value_type>&&;
        };

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------

        template <typename value_type, typename signature>
        struct is_function
        {
            static constexpr bool value = false;
        };

        template <typename value_type, typename return_type, typename... arg_types>
        struct is_function<value_type, return_type(arg_types...)>
        {
            static constexpr bool value =
                std::is_invocable_r_v<return_type, value_type, arg_types...>;
        };

        template <typename value_type, typename return_type, typename... arg_types>
        struct is_function<value_type, return_type(arg_types...) const>
        {
            static constexpr bool value =
                std::is_invocable_r_v<return_type, const value_type, arg_types...>;
        };

        template <typename value_type, typename return_type, typename... arg_types>
        struct is_function<value_type, return_type(arg_types...) volatile>
        {
            static constexpr bool value =
                std::is_invocable_r_v<return_type, volatile value_type, arg_types...>;
        };

        template <typename value_type, typename return_type, typename... arg_types>
        struct is_function<value_type, return_type(arg_types...) const volatile>
        {
            static constexpr bool value =
                std::is_invocable_r_v<return_type, const volatile value_type, arg_types...>;
        };
    };
}
