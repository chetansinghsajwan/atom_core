#pragma once
#include "_ArrImplHelper.decl.h"
#include "_ConstArrImplHelper.h"

namespace Atom
{
    template <typename TImpl>
    constexpr fn _ArrImplHelper<TImpl>::ElemAt(usize index) -> TElem&
    {
        ATOM_EXPECTS(_ValidateIndexForAccess(index)) <<
            IndexOutOfRangeException(index, 0, _Count() - 1);

        return _Data()[index];
    }

    template <typename TImpl>
    constexpr fn _ArrImplHelper<TImpl>::op[](usize index) -> TElem&
    {
        ATOM_DEBUG_EXPECTS(_ValidateIndexForAccess(index)) <<
            IndexOutOfRangeException(index, 0, _Count() - 1);

        return _Data()[index];
    }

    template <typename TImpl>
    constexpr fn _ArrImplHelper<TImpl>::ElemFront() -> TElem&
    {
        ATOM_DEBUG_EXPECTS(!isEmpty()) <<
            OutOfRangeException("Arr is empty.");

        return _Data()[0];
    }

    template <typename TImpl>
    constexpr fn _ArrImplHelper<TImpl>::ElemBack() -> TElem&
    {
        ATOM_DEBUG_EXPECTS(!isEmpty()) <<
            OutOfRangeException("Arr is empty.");

        return _Data()[_Count() - 1];
    }
}