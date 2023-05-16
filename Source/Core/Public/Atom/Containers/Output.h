#pragma once
#include "Ranges.h"
#include "RangesMock.h"

namespace Atom
{
    template <typename TOut, typename T>
    concept ROutput = requires(TOut out)
    {
        { out += declval(const T&) };
        { out += declval(T&&) };

        { out += Internal::FwdRangeMock<T>() };
    };
}