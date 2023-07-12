#pragma once
#include "ArrView.h"

namespace Atom
{
    template <tname T>
    class ArrLit: public ArrView<T>
    {
    public:
        template <usize size>
        consteval ctor ArrLit(T(&arr)[size]) noex:
            ArrView{ arr, size } { }
    };
}