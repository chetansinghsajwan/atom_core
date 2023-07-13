#pragma once
#include "_ArrImplHelper.decl.h"

namespace Atom
{
    template <tname T, usize Size>
    class _StaArrImplBase
    {
        pro using TElem = T;

        pro cexpr fn _Data() noex -> T*
        {
            return _arr;
        }

        pro cexpr fn _Data() const noex -> const T*
        {
            return _arr;
        }

        pro cexpr fn _Count() const noex -> usize
        {
            return Size;
        }

        pub T _arr[Size];
    };

    template <tname T, usize Size>
    using StaArr = _ArrImplHelper<_StaArrImplBase<T, Size>>;
}