#pragma once
#include "Atom/TTI.h"
#include "Atom/Str.h"

namespace Atom
{
//// -----------------------------------------------------------------------------------------------
//// LogStr Requirements
//// -----------------------------------------------------------------------------------------------

    template <typename... TArgs>
    using LogStr = FmtStr<TArgs...>;

    template <typename T>
    concept RLogArg = requires(T arg)
    {
        requires RConvertibleTo<T, StrView>;
    };
}