#pragma once
#include "ArrView.h"

namespace Atom
{
    template <typename T>
    class ArrLit: public ArrView<T>
    {
    public:
        template <usize size>
        consteval ctor ArrLit(T(&arr)[size]):
            ArrView{ arr, size } { }
    };
}