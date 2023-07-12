#pragma once
#include "Atom/Core.h"
#include "Atom/TTI.h"

namespace Atom
{
    namespace Private
    {
        template <tname TInvokable, tname... TSignature>
        struct IsInvokableImpl;

        template <tname TInvokable, tname TResult, tname... TArgs>
        struct IsInvokableImpl<TInvokable, TResult(TArgs...)>
        {
            static constexpr bool Value = ::std::is_invocable_r_v<
                TResult, TInvokable, TArgs...>;
        };

        /// @TODO Add impl for const invocable.
        template <tname TInvokable, tname TResult, tname... TArgs>
        struct IsInvokableImpl<TInvokable, TResult(TArgs...) const>
        {
            static constexpr bool Value = ::std::is_invocable_r_v<
                TResult, TInvokable, TArgs...>;
        };

        template <tname TInvokable, tname TResult, tname... TArgs>
        struct IsInvokableImpl<TInvokable, TResult(TArgs...) noex>
        {
            static constexpr bool Value = ::std::is_nothrow_invocable_r_v<
                TResult, TInvokable, TArgs...>;
        };

        /// @TODO Add impl for const invocable.
        template <tname TInvokable, tname TResult, tname... TArgs>
        struct IsInvokableImpl<TInvokable, TResult(TArgs...) const noex>
        {
            static constexpr bool Value = ::std::is_nothrow_invocable_r_v<
                TResult, TInvokable, TArgs...>;
        };
    }

    template <tname TInvokable, tname... TSignature>
    concept RInvokable = Private::IsInvokableImpl<TInvokable, TSignature...>::Value;
}