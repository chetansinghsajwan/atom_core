#pragma once
#include "_ArrImplHelper.decl.h"

namespace Atom
{
    template <typename T, usize Size>
    class _StaticArrImplBase
    {
    protected:
        using TElem = T;

    protected:
        constexpr auto _Data() -> T*
        {
            return _arr;
        }

        constexpr auto _Data() const -> const T*
        {
            return _arr;
        }

        constexpr auto _Count() const -> usize
        {
            return Size;
        }

    public:
        T _arr[Size];
    };

    template <typename T, usize Size>
    using StaticArr = _ArrImplHelper<_StaticArrImplBase<T, Size>>;
}
