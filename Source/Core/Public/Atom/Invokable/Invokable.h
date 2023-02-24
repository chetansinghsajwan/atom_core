#pragma once
#include "Atom/Core.h"
#include "Atom/TTI.h"

namespace Atom
{
    template <typename InvokableType, typename ResultType, typename... Args>
    concept Invokable = requires(InvokableType invokable, Args... args)
    {
        requires std::invocable<InvokableType, Args...>;

        { invokable(FORWARD(args)...) } -> TTI::SameAs<ResultType>;
    };
}