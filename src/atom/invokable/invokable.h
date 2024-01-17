#pragma once
#include "atom/core.h"
#include "atom/tti.h"

namespace atom
{
    template <typename tinvokable, typename... tsignature>
    class _is_invokable_impl;

    template <typename tinvokable, typename result_type, typename... arg_types>
    class _is_invokable_impl<tinvokable, result_type(arg_types...)>
    {
    public:
        static constexpr bool value = std::is_invocable_r_v<result_type, tinvokable, arg_types...>;
    };

    /// @todo add impl for const invocable.
    template <typename tinvokable, typename result_type, typename... arg_types>
    class _is_invokable_impl<tinvokable, result_type(arg_types...) const>
    {
    public:
        static constexpr bool value = std::is_invocable_r_v<result_type, tinvokable, arg_types...>;
    };

    template <typename tinvokable, typename... tsignature>
    concept rinvokable = _is_invokable_impl<tinvokable, tsignature...>::value;
}
