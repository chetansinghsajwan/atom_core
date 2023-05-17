#pragma once
#include "Output.h"

namespace Atom::Internal
{
    template <typename T>
    struct OutputMock
    {
        void operator += (const T& in);
        void operator += (T&& in);

        void operator += (RFwdIter<T> auto in);
    };
}
