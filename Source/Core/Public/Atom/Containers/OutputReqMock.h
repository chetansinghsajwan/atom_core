#pragma once
#include "OutputReq.h"

namespace Atom
{
    template <tname T>
    class OutputReqMock
    {
        pub template <tname U>
        requires RSameAsUnqualified<U, T>
        fn operator +=(U&& el);

        pub template <tname TRange>
        requires RRangeOf<TRange, T>
        fn operator +=(const TRange& range);
    };
}
