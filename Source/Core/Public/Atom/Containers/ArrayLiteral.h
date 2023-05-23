#pragma once
#include "ArrayView.h"

namespace Atom
{
    template <typename T>
    class ArrayLiteral: public ArrayView<T>
    {
        template <usize size>
        consteval ArrayLiteral(T(&arr)[size]) noexcept:
            ArrayView{ arr, size } { }

        template <usize size>
        consteval ArrayLiteral(const ArrayView& view) noexcept:
            ArrayView{ view } { }
    };
}