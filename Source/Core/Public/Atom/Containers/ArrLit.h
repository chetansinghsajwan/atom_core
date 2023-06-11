#pragma once
#include "ArrView.h"

namespace Atom
{
    template <typename T>
    class ArrLit: pub ArrView<T>
    {
        pubm template <usize size>
        ceval ArrLit(T(&arr)[size]) noexcept:
            ArrView{ arr, size } { }
    };
}