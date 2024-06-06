export module atom.core:types.typeinfo_impl;

import std;

namespace atom
{
    namespace typeinfo_impl
    {
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

        template <typename value_type, typename return_type, typename... args_type>
        struct is_function<value_type, return_type(args_type...)>
        {
            static constexpr bool value = std::is_invocable_r_v<return_type, value_type, args_type...>;
        };

        template <typename value_type, typename return_type, typename... args_type>
        struct is_function<value_type, return_type(args_type...) const>
        {
            static constexpr bool value = std::is_invocable_r_v<return_type, const value_type, args_type...>;
        };

        template <typename value_type, typename return_type, typename... args_type>
        struct is_function<value_type, return_type(args_type...) volatile>
        {
            static constexpr bool value =
                std::is_invocable_r_v<return_type, volatile value_type, args_type...>;
        };

        template <typename value_type, typename return_type, typename... args_type>
        struct is_function<value_type, return_type(args_type...) const volatile>
        {
            static constexpr bool value =
                std::is_invocable_r_v<return_type, const volatile value_type, args_type...>;
        };
    };
}
