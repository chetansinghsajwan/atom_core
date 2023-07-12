#pragma once
#include "_ArrImplHelper.decl.h"

namespace Atom
{
    template <typename T, usize Size>
    struct _StaArrImplBase
    {
        using TElem = T;

        constexpr T* _Data() noex
        {
            return _arr;
        }

        constexpr const T* _Data() const noex
        {
            return _arr;
        }

        constexpr usize _Count() const noex
        {
            return Size;
        }

        T _arr[Size];
    };

    template <typename T, usize Size>
    using StaArr = _ArrImplHelper<_StaArrImplBase<T, Size>>;
}