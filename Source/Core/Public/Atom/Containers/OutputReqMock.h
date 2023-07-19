#pragma once
#include "OutputReq.h"

namespace Atom
{
    template <tname T>
    class OutputReqMock
    {
        pub template <tname U>
        requires RSameAsUnqualified<U, T>
        fn op+=(U&& el);

        pub template <tname TRange>
        requires RRangeOf<TRange, T>
        fn op+=(const TRange& range);
    };
}
