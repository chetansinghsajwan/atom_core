#pragma once
#include "Atom/Core.h"
#include "Atom/TTI.h"

namespace Atom
{
    namespace PrivateInvokable
    {
        template <typename TInvokable, typename TResult, typename... TArgs>
        concept RInvokable = requires(TInvokable invokable, TArgs... args)
        {
            requires std::invocable<TInvokable, TArgs...>;

            { invokable(FORWARD(args)...) } -> TTI::RSameAs<TResult>;
        };

        template <typename TInvokable, typename TResult, typename... TArgs>
        concept RInvokableConst = requires(const TInvokable invokable, TArgs... args)
        {
            requires std::invocable<TInvokable, TArgs...>;

            { invokable(FORWARD(args)...) } -> TTI::RSameAs<TResult>;
        };

        template <typename TInvokable, typename TResult, typename... TArgs>
        concept RInvokableNoexcept = requires(TInvokable invokable, TArgs... args)
        {
            requires std::invocable<TInvokable, TArgs...>;

            { invokable(FORWARD(args)...) } noexcept -> TTI::RSameAs<TResult>;
        };

        template <typename TInvokable, typename TResult, typename... TArgs>
        concept RInvokableConstNoexcept = requires(const TInvokable invokable, TArgs... args)
        {
            requires std::invocable<TInvokable, TArgs...>;

            { invokable(FORWARD(args)...) } noexcept -> TTI::RSameAs<TResult>;
        };
    }

    template <typename TInvokable, typename TResult, typename... TArgs>
    struct RInvokable;

    template <typename TInvokable, typename TResult, typename... TArgs>
    struct RInvokable <TInvokable, TResult(TArgs...)>
    {
        static constinit const bool Value = PrivateInvokable::RInvokable<
            TInvokable, TResult, TArgs...>;
    };

    template <typename TInvokable, typename TResult, typename... TArgs>
    struct RInvokable <TInvokable, TResult(TArgs...) const>
    {
        static constinit const bool Value = PrivateInvokable::RInvokableConst<
            TInvokable, TResult, TArgs...>;
    };

    template <typename TInvokable, typename TResult, typename... TArgs>
    struct RInvokable <TInvokable, TResult(TArgs...) noexcept>
    {
        static constinit const bool Value = PrivateInvokable::RInvokableNoexcept<
            TInvokable, TResult, TArgs...>;
    };

    template <typename TInvokable, typename TResult, typename... TArgs>
    struct RInvokable <TInvokable, TResult(TArgs...) const noexcept>
    {
        static constinit const bool Value = PrivateInvokable::RInvokableConstNoexcept<
            TInvokable, TResult, TArgs...>;
    };
}