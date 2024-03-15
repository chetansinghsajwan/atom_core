#pragma once
#include "atom/core/invokable/invokable.h"

namespace atom
{
    template <typename... tsign>
    class _invokable_ptr_impl;

    template <typename result_t, typename... arg_ts>
    class _invokable_ptr_impl<result_t(arg_ts...)>
    {
    public:
        using type = result_t (*)(arg_ts...);
    };

    template <typename... tsign>
    using invokable_ptr = typename _invokable_ptr_impl<tsign...>::type;
}
