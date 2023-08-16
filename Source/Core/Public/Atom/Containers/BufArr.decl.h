#pragma once
#include "DynArr.decl.h"

namespace Atom
{
    template <typename TAlloc>
    class _BufArrAllocWrap: public TAlloc {};

    template <typename T, usize bufSize, typename TAlloc>
    class BufArr: public DynArr<T, _BufArrAllocWrap<TAlloc>>
    {
        using Base = DynArr<T, _BufArrAllocWrap<TAlloc>>;

    public:
        using Base::Base;
        using Base::operator=;
    };
}
