#pragma once
#include "Atom/TTI.h"
#include "Atom/String.h"

namespace Atom
{
//// -----------------------------------------------------------------------------------------------
//// LogString Requirements
//// -----------------------------------------------------------------------------------------------

    template <typename... TArgs>
    using LogString = Fmt::FormatString<TArgs...>;

    template <typename T>
    concept RLogArg = requires(T arg)
    {
        requires TTI::RConvertible<T, StringView>;
    };
}