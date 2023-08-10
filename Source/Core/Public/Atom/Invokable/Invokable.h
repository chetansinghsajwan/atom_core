#pragma once
#include "Atom/Core.h"
#include "Atom/TTI.h"

namespace Atom
{
    namespace Private
    {
        template <typename TInvokable, typename... TSignature>
        class IsInvokableImpl;

        template <typename TInvokable, typename TResult, typename... TArgs>
        class IsInvokableImpl<TInvokable, TResult(TArgs...)>
        {
        public:
            static constexpr bool Value = std::is_invocable_r_v<
                TResult, TInvokable, TArgs...>;
        };

        /// @TODO Add impl for const invocable.
        template <typename TInvokable, typename TResult, typename... TArgs>
        class IsInvokableImpl<TInvokable, TResult(TArgs...) const>
        {
        public:
            static constexpr bool Value = std::is_invocable_r_v<
                TResult, TInvokable, TArgs...>;
        };
    }

    template <typename TInvokable, typename... TSignature>
    concept RInvokable = Private::IsInvokableImpl<TInvokable, TSignature...>::Value;
}