#pragma once
#include "OutputReq.h"

namespace Atom
{
    template <typename T>
    class OutputReqMock
    {
    public:
        template <typename U>
        fn operator+=(U&& el)
            requires(RSameAsUnqualified<U, T>);

    public:
        template <typename TRange>
        fn operator+=(const TRange& range)
            requires(RRangeOf<TRange, T>);
    };
}
