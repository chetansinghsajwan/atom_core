#pragma once
#include "Output.h"

namespace Atom::Internal
{
    template <typename T>
    struct OutputMock
    {
        void operator += (const T& in);
        void operator += (T&& in);

        template <typename TRange>
        requires RFwdRange<TRange, const T>
        void operator += (const TRange& range);
    };
}
