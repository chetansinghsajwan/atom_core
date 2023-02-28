#pragma once
#include "Atom/TTI.h"
#include "Atom/String.h"

namespace Atom
{
//// -----------------------------------------------------------------------------------------------
//// LogString Requirements
//// -----------------------------------------------------------------------------------------------

    template <typename T>
    concept RLogString = requires(T str)
    {
        requires TTI::RConvertible<T, StringView>;
    };

    template <typename T>
    concept RLogArg = requires(T arg)
    {
        requires TTI::RConvertible<T, StringView>;
    };
}