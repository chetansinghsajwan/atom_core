#pragma once
#include "OutputReq.h"

namespace Atom
{
    template <typename T>
    class OutputReqMock
    {
        pub template <typename U>
        requires RSameAsUnqualified<U, T>
        fn op+=(U&& el);

        pub template <typename TRange>
        requires RRangeOf<TRange, T>
        fn op+=(const TRange& range);
    };
}
