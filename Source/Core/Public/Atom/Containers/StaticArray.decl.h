#pragma once
#include "_ArrImplHelper.decl.h"

namespace Atom
{
    template <typename T, usize Size>
    struct _StaticArrImplBase
    {
        using TElem = T;

        constexpr T* _Data() noexcept
        {
            return _arr;
        }

        constexpr const T* _Data() const noexcept
        {
            return _arr;
        }

        constexpr usize _Count() const noexcept
        {
            return Size;
        }

        T _arr[Size];
    };

    template <typename T, usize Size>
    using StaticArray = _ArrImplHelper<_StaticArrImplBase<T, Size>>;
}