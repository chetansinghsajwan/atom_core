#pragma once
#include "Atom/Range.h"

namespace Atom
{
    template <typename TOut, typename T>
    concept ROutput = requires(TOut out, const T& inCopy, T&& inMove, RangeReqMock<T> inRange)
    {
        { out += inCopy };
        { out += mov(inMove) };
        { out += inRange };
    };
}