#pragma once
#include "OutputReq.h"

namespace Atom
{
    template <typename T>
    struct OutputReqMock
    {
        template <typename U>
        requires RSameAsUnqualified<U, T>
        void operator +=(U&& el);

        template <typename TRange>
        requires RRange<TRange, T>
        void operator +=(const TRange& range);
    };
}
