#pragma once
#include "OutputReq.h"
#include "InsertableReq.h"
#include "Atom/Range.h"

namespace Atom
{
    template <tname TImpl>
    class _ConstArrImplHelper: public TImpl
    {
    //// -------------------------------------------------------------------------------------------
    //// Aliases
    //// -------------------------------------------------------------------------------------------

    public:
        using TElem = tname TImpl::TElem;
        using TIter = ArrIter<TElem>;
        using TIterEnd = TIter;

    //// -------------------------------------------------------------------------------------------
    //// Access
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// Access element at index{index}.
        /// 
        /// @PARAM index: Index of element to access.
        /// 
        /// @RETURNS Const ref to element.
        /// 
        /// @EXPECTS {index} is in range[0, Count() - 1].
        ///     @THROWS IndexOutOfRangeException.
        /// 
        /// @TIME_COMPLEXITY Constant.
        /// ----------------------------------------------------------------------------------------
        cexpr fn ElemAt(usize index) const -> const TElem&;

        /// ----------------------------------------------------------------------------------------
        /// Access element at index{index}. Same as {ElemAt(...)}, but doesn't perform bounds checking
        /// in Release.
        /// 
        /// @PARAM index: Index of element to access.
        /// 
        /// @RETURNS Const ref to element.
        /// 
        /// @EXPECTS {index} is in range[0, Count() - 1].
        ///     @THROWS IndexOutOfRangeException: In Debug.
        /// 
        /// @TIME_COMPLEXITY Constant.
        /// ----------------------------------------------------------------------------------------
        cexpr fn op[](usize index) const noex -> const TElem&;

        /// ----------------------------------------------------------------------------------------
        /// Access first element.
        /// 
        /// @RETURNS Const ref to element.
        /// 
        /// @EXPECTS {!IsEmpty()}.
        ///     @THROWS OutOfRangeException: In Debug.
        /// 
        /// @TIME_COMPLEXITY Constant.
        /// ----------------------------------------------------------------------------------------
        cexpr fn ElemFront() const noex -> const TElem&;

        /// ----------------------------------------------------------------------------------------
        /// Access last element.
        /// 
        /// @RETURNS Const ref to element.
        /// 
        /// @EXPECTS {!IsEmpty()}.
        ///     @THROWS OutOfRangeException: In Debug.
        /// 
        /// @TIME_COMPLEXITY Constant.
        /// ----------------------------------------------------------------------------------------
        cexpr fn ElemBack() const noex -> const TElem&;

        /// ----------------------------------------------------------------------------------------
        /// Pointer to underlying arr.
        /// 
        /// @RETURNS Const ptr to underlying arr.
        /// 
        /// @TIME_COMPLEXITY Constant.
        /// ----------------------------------------------------------------------------------------
        cexpr fn Data() const noex -> const TElem*
        {
            return _Data();
        }

        /// ----------------------------------------------------------------------------------------
        /// Get count of elements.
        /// ----------------------------------------------------------------------------------------
        cexpr fn Count() const noex -> usize
        {
            return _Count();
        }

        /// ----------------------------------------------------------------------------------------
        /// Is arr empty.
        /// ----------------------------------------------------------------------------------------
        cexpr fn IsEmpty() const noex -> bool
        {
            return _Count() == 0;
        }

    //// -------------------------------------------------------------------------------------------
    //// Iteration
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// {TIter} to the first element.
        /// ----------------------------------------------------------------------------------------
        cexpr fn Iter() const noex -> TIter
        {
            return TIter{ _Data() };
        }

        /// ----------------------------------------------------------------------------------------
        /// {TIter} to element following the last element.
        /// ----------------------------------------------------------------------------------------
        cexpr fn IterEnd() const noex -> TIter
        {
            return TIter{ _Data() + _Count()};
        }

    //// -------------------------------------------------------------------------------------------
    //// Implementations
    //// -------------------------------------------------------------------------------------------

    protected:
        cexpr fn _ValidateIndexForAccess(isize index) const noex -> bool
        {
            return index > 0 && index < (isize)_Count();
        }

        using TImpl::_Data;
        using TImpl::_Count;
    };
}