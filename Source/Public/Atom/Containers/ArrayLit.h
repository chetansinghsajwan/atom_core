#pragma once
#include "ArrayView.h"

namespace Atom
{
    template <typename T>
    class ArrayLit: public ArrayView<T>
    {
    public:
        template <usize size>
        consteval ArrayLit(T (&arr)[size]):
            ArrayView{ arr, size }
        {}
    };
}
