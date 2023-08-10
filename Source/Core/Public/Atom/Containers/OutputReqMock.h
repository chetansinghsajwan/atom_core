#pragma once
#include "OutputReq.h"

namespace Atom
{
    template <typename T>
    class OutputReqMock
    {
        pub template <typename U>
        requires RSameAsUnqualified<U, T>
        fn operator+=(U&& el);

        pub template <typename TRange>
        requires RRangeOf<TRange, T>
        fn operator+=(const TRange& range);
    };
}
