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

        template <typename in_value_t, typename other_t>
        struct unpure_like_t
        {
            using value_t = std::remove_cvref_t<in_value_t>;
        };

        template <typename in_value_t, typename other_t>
        struct unpure_like_t<in_value_t, const other_t>
        {
            using value_t = const std::remove_cvref_t<in_value_t>;
        };

        template <typename in_value_t, typename other_t>
        struct unpure_like_t<in_value_t, volatile other_t>
        {
            using value_t = volatile std::remove_cvref_t<in_value_t>;
        };

        template <typename in_value_t, typename other_t>
        struct unpure_like_t<in_value_t, const volatile other_t>
        {
            using value_t = const volatile std::remove_cvref_t<in_value_t>;
        };

        template <typename in_value_t, typename other_t>
        struct unpure_like_t<in_value_t, other_t&>
        {
            using value_t = std::remove_cvref_t<in_value_t>&;
        };

        template <typename in_value_t, typename other_t>
        struct unpure_like_t<in_value_t, other_t&&>
        {
            using value_t = std::remove_cvref_t<in_value_t>&&;
        };

        template <typename in_value_t, typename other_t>
        struct unpure_like_t<in_value_t, const other_t&>
        {
            using value_t = const std::remove_cvref_t<in_value_t>&;
        };

        template <typename in_value_t, typename other_t>
        struct unpure_like_t<in_value_t, const other_t&&>
        {
            using value_t = const std::remove_cvref_t<in_value_t>&&;
        };

        template <typename in_value_t, typename other_t>
        struct unpure_like_t<in_value_t, volatile other_t&>
        {
            using value_t = volatile std::remove_cvref_t<in_value_t>&;
        };

        template <typename in_value_t, typename other_t>
        struct unpure_like_t<in_value_t, volatile other_t&&>
        {
            using value_t = volatile std::remove_cvref_t<in_value_t>&&;
        };

        template <typename in_value_t, typename other_t>
        struct unpure_like_t<in_value_t, const volatile other_t&>
        {
            using value_t = const volatile std::remove_cvref_t<in_value_t>&;
        };

        template <typename in_value_t, typename other_t>
        struct unpure_like_t<in_value_t, const volatile other_t&&>
        {
            using value_t = const volatile std::remove_cvref_t<in_value_t>&&;
        };

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------

        template <typename value_t, typename signature>
        struct is_invokable
        {
            static constexpr bool value = false;
        };

        template <typename value_t, typename return_t, typename... args_t>
        struct is_invokable<value_t, return_t(args_t...)>
        {
            static constexpr bool value = std::is_invocable_r_v<return_t, value_t, args_t...>;
        };

        template <typename value_t, typename return_t, typename... args_t>
        struct is_invokable<value_t, return_t(args_t...) const>
        {
            static constexpr bool value = std::is_invocable_r_v<return_t, const value_t, args_t...>;
        };

        template <typename value_t, typename return_t, typename... args_t>
        struct is_invokable<value_t, return_t(args_t...) volatile>
        {
            static constexpr bool value =
                std::is_invocable_r_v<return_t, volatile value_t, args_t...>;
        };

        template <typename value_t, typename return_t, typename... args_t>
        struct is_invokable<value_t, return_t(args_t...) const volatile>
        {
            static constexpr bool value =
                std::is_invocable_r_v<return_t, const volatile value_t, args_t...>;
        };
    };
}
