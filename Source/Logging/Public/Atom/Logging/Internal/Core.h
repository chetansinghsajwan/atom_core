#pragma once
#include "Atom/TTI.h"

namespace Atom
{
//// -----------------------------------------------------------------------------------------------
//// LogString Requirements
//// -----------------------------------------------------------------------------------------------

    template <typename T>
    concept LogStringType = requires(T str)
    {
        requires TTI::ConvertibleTo<T, StringView>;
    };

    template <typename T>
    concept LogArgType = requires(T arg)
    {
        requires TTI::ConvertibleTo<T, StringView>;
    };
}