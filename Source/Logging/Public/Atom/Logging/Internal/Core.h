#pragma once
#include "Atom/Core.h"
#include "Atom/TTI.h"
#include "Atom/Str.h"

namespace Atom
{
//// -----------------------------------------------------------------------------------------------
//// LogStr Requirements
//// -----------------------------------------------------------------------------------------------

    template <tname... TArgs>
    using LogStr = FmtStr<TArgs...>;

    template <tname T>
    concept RLogArg = requires(T arg)
    {
        requires RConvertibleTo<T, StrView>;
    };
}