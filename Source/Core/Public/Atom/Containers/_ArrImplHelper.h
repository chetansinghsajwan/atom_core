#pragma once
#include "_ArrImplHelper.decl.h"
#include "_ConstArrImplHelper.h"

namespace Atom
{
    template <tname TImpl>
    cexpr fn _ArrImplHelper<TImpl>::ElemAt(usize index) -> TElem&
    {
        ATOM_EXPECTS(_ValidateIndexForAccess(index)) << IndexOutOfRangeException(
            ATOM_TEXT("{index} was out of range[{0}, {1}]."), index, 0, _Count() - 1);

        return _Data()[index];
    }

    template <tname TImpl>
    cexpr fn _ArrImplHelper<TImpl>::operator [](usize index) noex -> TElem&
    {
        ATOM_DEBUG_EXPECTS(_ValidateIndexForAccess(index)) << IndexOutOfRangeException(
            ATOM_TEXT("{index} was out of range[{0}, {1}]."), index, 0, _Count() - 1);

        return _Data()[index];
    }

    template <tname TImpl>
    cexpr fn _ArrImplHelper<TImpl>::ElemFront() noex -> TElem&
    {
        ATOM_DEBUG_EXPECTS(!IsEmpty()) << OutOfRangeException(
            ATOM_TEXT("Arr is empty."));

        return _Data()[0];
    }

    template <tname TImpl>
    cexpr fn _ArrImplHelper<TImpl>::ElemBack() noex -> TElem&
    {
        ATOM_DEBUG_EXPECTS(!IsEmpty()) << OutOfRangeException(
            ATOM_TEXT("Arr is empty."));

        return _Data()[_Count() - 1];
    }
}