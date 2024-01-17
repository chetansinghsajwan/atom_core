#pragma once
#include "array_view.h"

namespace atom
{
    template <typename type>
    class array_lit: public array_view<type>
    {
    public:
        template <usize size>
        consteval array_lit(type (&arr)[size])
            : array_view{ arr, size }
        {}
    };
}
