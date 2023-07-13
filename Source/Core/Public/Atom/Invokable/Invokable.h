#pragma once
#include "Atom/Core.h"
#include "Atom/TTI.h"

namespace Atom
{
    namespace Private
    {
        template <tname TInvokable, tname... TSignature>
        class IsInvokableImpl;

        template <tname TInvokable, tname TResult, tname... TArgs>
        class IsInvokableImpl<TInvokable, TResult(TArgs...)>
        {
            pub static cexpr bool Value = std::is_invocable_r_v<
                TResult, TInvokable, TArgs...>;
        };

        /// @TODO Add impl for const invocable.
        template <tname TInvokable, tname TResult, tname... TArgs>
        class IsInvokableImpl<TInvokable, TResult(TArgs...) const>
        {
            pub static cexpr bool Value = std::is_invocable_r_v<
                TResult, TInvokable, TArgs...>;
        };

        template <tname TInvokable, tname TResult, tname... TArgs>
        class IsInvokableImpl<TInvokable, TResult(TArgs...) noex>
        {
            pub static cexpr bool Value = std::is_nothrow_invocable_r_v<
                TResult, TInvokable, TArgs...>;
        };

        /// @TODO Add impl for const invocable.
        template <tname TInvokable, tname TResult, tname... TArgs>
        class IsInvokableImpl<TInvokable, TResult(TArgs...) const noex>
        {
            pub static cexpr bool Value = std::is_nothrow_invocable_r_v<
                TResult, TInvokable, TArgs...>;
        };
    }

    template <tname TInvokable, tname... TSignature>
    concept RInvokable = Private::IsInvokableImpl<TInvokable, TSignature...>::Value;
}