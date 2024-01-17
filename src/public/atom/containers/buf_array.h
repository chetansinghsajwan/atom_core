#pragma once
#include "DynamicArray.h"

namespace Atom
{
    template <typename TAlloc>
    class _BufArrayAllocWrap: public TAlloc
    {};

    template <typename T, usize bufSize, typename TAlloc>
    class BufArray: public DynamicArray<T, _BufArrayAllocWrap<TAlloc>>
    {
        using Base = DynamicArray<T, _BufArrayAllocWrap<TAlloc>>;

    public:
        using Base::Base;
        using Base::operator=;
    };
}
