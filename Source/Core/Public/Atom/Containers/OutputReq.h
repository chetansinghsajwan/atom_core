#pragma once
#include "RangeReq.h"
#include "RangeReqMock.h"

namespace Atom
{
    template <typename TOut, typename T>
    concept ROutput = requires(TOut out)
    {
        { out += declval(const T&) };
        { out += declval(T&&) };

        { out += RangeReqMock<T>() };
    };
}