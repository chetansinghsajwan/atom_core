#pragma once
#include "_ArrImplHelper.decl.h"
#include "_ConstArrImplHelper.h"

namespace Atom
{
    template <tname TImpl>
    cexpr fn _ArrImplHelper<TImpl>::ElemAt(usize index) -> TElem&
    {
        ATOM_EXPECTS(_ValidateIndexForAccess(index)) <<
            IndexOutOfRangeException(index, 0, _Count() - 1);

        return _Data()[index];
    }

    template <tname TImpl>
    cexpr fn _ArrImplHelper<TImpl>::op[](usize index) noex -> TElem&
    {
        ATOM_DEBUG_EXPECTS(_ValidateIndexForAccess(index)) <<
            IndexOutOfRangeException(index, 0, _Count() - 1);

        return _Data()[index];
    }

    template <tname TImpl>
    cexpr fn _ArrImplHelper<TImpl>::ElemFront() noex -> TElem&
    {
        ATOM_DEBUG_EXPECTS(!IsEmpty()) <<
            OutOfRangeException("Arr is empty.");

        return _Data()[0];
    }

    template <tname TImpl>
    cexpr fn _ArrImplHelper<TImpl>::ElemBack() noex -> TElem&
    {
        ATOM_DEBUG_EXPECTS(!IsEmpty()) <<
            OutOfRangeException("Arr is empty.");

        return _Data()[_Count() - 1];
    }
}