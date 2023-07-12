#pragma once
#include "Atom/Range.h"

namespace Atom
{
    template <tname TOut, tname T>
    concept ROutput = requires(TOut out)
    {
        { out += declval(const T&) };
        { out += declval(T&&) };

        { out += RangeReqMock<T>() };
    };
}