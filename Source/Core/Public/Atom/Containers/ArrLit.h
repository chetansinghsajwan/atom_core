#pragma once
#include "ArrView.h"

namespace Atom
{
    template <typename T>
    class ArrLit: public ArrView<T>
    {
        template <usize size>
        consteval ArrLit(T(&arr)[size]) noexcept:
            ArrView{ arr, size } { }

        template <usize size>
        consteval ArrLit(const ArrView& view) noexcept:
            ArrView{ view } { }
    };
}