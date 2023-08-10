#pragma once
#include "_ArrImplHelper.decl.h"

namespace Atom
{
    template <typename T, usize Size>
    class _StaticArrImplBase
    {
        prot using TElem = T;

        prot constexpr fn _Data() -> T*
        {
            return _arr;
        }

        prot constexpr fn _Data() const -> const T*
        {
            return _arr;
        }

        prot constexpr fn _Count() const -> usize
        {
            return Size;
        }

        pub T _arr[Size];
    };

    template <typename T, usize Size>
    using StaticArr = _ArrImplHelper<_StaticArrImplBase<T, Size>>;
}