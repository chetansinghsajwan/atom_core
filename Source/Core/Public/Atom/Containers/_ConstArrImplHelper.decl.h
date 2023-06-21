#pragma once
#include "OutputReq.h"
#include "InsertableReq.h"
#include "Atom/Range.h"

namespace Atom
{
    template <typename _TImpl>
    class _ConstArrImplHelper: public _TImpl
    {
    //// -------------------------------------------------------------------------------------------
    //// Aliases
    //// -------------------------------------------------------------------------------------------

    public:
        using TElem = typename _TImpl::TElem;
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
        constexpr auto ElemAt(usize index) const
            -> const TElem&;

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
        constexpr auto operator [](usize index) const noexcept
            -> const TElem&;

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
        constexpr auto ElemFront() const noexcept
            -> const TElem&;

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
        constexpr auto ElemBack() const noexcept
            -> const TElem&;

        /// ----------------------------------------------------------------------------------------
        /// Pointer to underlying arr.
        /// 
        /// @RETURNS Const ptr to underlying arr.
        /// 
        /// @TIME_COMPLEXITY Constant.
        /// ----------------------------------------------------------------------------------------
        constexpr auto Data() const noexcept
            -> const TElem*
        {
            return _Data();
        }

        /// ----------------------------------------------------------------------------------------
        /// Get count of elements.
        /// ----------------------------------------------------------------------------------------
        constexpr auto Count() const noexcept
            -> usize
        {
            return _Count();
        }

        /// ----------------------------------------------------------------------------------------
        /// Is arr empty.
        /// ----------------------------------------------------------------------------------------
        constexpr auto IsEmpty() const noexcept
            -> bool
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
        constexpr auto Iter() const noexcept
            -> TIter
        {
            return TIter{ _Data() };
        }

        /// ----------------------------------------------------------------------------------------
        /// {TIter} to element following the last element.
        /// ----------------------------------------------------------------------------------------
        constexpr auto IterEnd() const noexcept
            -> TIter
        {
            return TIter{ _Data() + _Count()};
        }

    //// -------------------------------------------------------------------------------------------
    //// Implementations
    //// -------------------------------------------------------------------------------------------

    protected:
        constexpr auto _ValidateIndexForAccess(isize index) const noexcept
            -> bool
        {
            return index > 0 && index < (isize)_Count();
        }

        prom using _TImpl::_Data;
        prom using _TImpl::_Count;
    };
}