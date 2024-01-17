#pragma once
#include "atom/core.h"
#include "atom/tti.h"

namespace atom
{
    template <typename tinvokable, typename... tsignature>
    class _is_invokable_impl;

    template <typename tinvokable, typename result_type, typename... args_type>
    class _is_invokable_impl<tinvokable, result_type(args_type...)>
    {
    public:
        static constexpr bool value = std::is_invocable_r_v<result_type, tinvokable, args_type...>;
    };

    /// @todo add impl for const invocable.
    template <typename tinvokable, typename result_type, typename... args_type>
    class _is_invokable_impl<tinvokable, result_type(args_type...) const>
    {
    public:
        static constexpr bool value = std::is_invocable_r_v<result_type, tinvokable, args_type...>;
    };

    template <typename tinvokable, typename... tsignature>
    concept rinvokable = _is_invokable_impl<tinvokable, tsignature...>::value;
}
