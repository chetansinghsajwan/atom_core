#pragma once
#include "_ArrImplHelper.decl.h"

namespace Atom
{
    template <tname T, usize Size>
    struct _StaArrImplBase
    {
        using TElem = T;

        cexpr fn _Data() noex -> T*
        {
            return _arr;
        }

        cexpr fn _Data() const noex -> const T*
        {
            return _arr;
        }

        cexpr fn _Count() const noex -> usize
        {
            return Size;
        }

        T _arr[Size];
    };

    template <tname T, usize Size>
    using StaArr = _ArrImplHelper<_StaArrImplBase<T, Size>>;
}