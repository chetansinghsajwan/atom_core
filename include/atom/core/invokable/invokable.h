#pragma once
#include "atom/core/_std.h"

namespace atom
{
    template <typename invokable_t, typename... tsignature>
    class _is_invokable_impl;

    template <typename invokable_t, typename result_t, typename... arg_ts>
    class _is_invokable_impl<invokable_t, result_t(arg_ts...)>
    {
    public:
        static constexpr bool value =
            std::is_invocable_r_v<result_t, invokable_t, arg_ts...>;
    };

    /// @todo add impl for const invocable.
    template <typename invokable_t, typename result_t, typename... arg_ts>
    class _is_invokable_impl<invokable_t, result_t(arg_ts...) const>
    {
    public:
        static constexpr bool value =
            std::is_invocable_r_v<result_t, invokable_t, arg_ts...>;
    };

    template <typename invokable_t, typename... tsignature>
    concept is_invokable = _is_invokable_impl<invokable_t, tsignature...>::value;
}
