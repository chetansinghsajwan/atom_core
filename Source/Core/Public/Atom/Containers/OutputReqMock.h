#pragma once
#include "OutputReq.h"

namespace Atom
{
    template <tname T>
    struct OutputReqMock
    {
        template <tname U>
        requires RSameAsUnqualified<U, T>
        void operator +=(U&& el);

        template <tname TRange>
        requires RRangeOf<TRange, T>
        void operator +=(const TRange& range);
    };
}
