#pragma once
#include "OutputReq.h"

namespace Atom
{
    template <tname T>
    struct OutputReqMock
    {
        template <tname U>
        requires RSameAsUnqualified<U, T>
        fn operator +=(U&& el);

        template <tname TRange>
        requires RRangeOf<TRange, T>
        fn operator +=(const TRange& range);
    };
}
