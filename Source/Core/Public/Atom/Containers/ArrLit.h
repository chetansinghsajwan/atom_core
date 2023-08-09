#pragma once
#include "ArrView.h"

namespace Atom
{
    template <typename T>
    class ArrLit: public ArrView<T>
    {
    public:
        template <usize size>
        ceval ctor ArrLit(T(&arr)[size]) noex:
            ArrView{ arr, size } { }
    };
}