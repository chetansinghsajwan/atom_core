#pragma once
#include "Atom/Range.h"
#include "InsertableReq.h"
#include "OutputReq.h"

namespace Atom
{
    template <typename TImpl>
    class _ConstArrImplHelper: public TImpl
    {
        ////
        ///-------------------------------------------------------------------------------------------
        //// Aliases
        ////
        ///-------------------------------------------------------------------------------------------

    public:
        using TElem = typename TImpl::TElem;
        using TIter = ArrIter<TElem>;
        using TIterEnd = TIter;

        ////
        ///-------------------------------------------------------------------------------------------
        //// Access
        ////
        ///-------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// Access element at index{index}.
        ///
        /// @PARAM index: Index of element to access.
        ///
        /// @RETURNS Const ref to element.
        ///
        /// @EXPECTS {index} is in range[0, count() - 1].
        ///     @THROWS IndexOutOfRangeException.
        ///
        /// @TIME_COMPLEXITY Constant.
        /// ----------------------------------------------------------------------------------------
        constexpr auto ElemAt(usize index) const -> const TElem&;

        /// ----------------------------------------------------------------------------------------
        /// Access element at index{index}. Same as {ElemAt(...)}, but doesn't perform bounds
        /// checking in Release.
        ///
        /// @PARAM index: Index of element to access.
        ///
        /// @RETURNS Const ref to element.
        ///
        /// @EXPECTS {index} is in range[0, count() - 1].
        ///     @THROWS IndexOutOfRangeException: In Debug.
        ///
        /// @TIME_COMPLEXITY Constant.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator[](usize index) const -> const TElem&;

        /// ----------------------------------------------------------------------------------------
        /// Access first element.
        ///
        /// @RETURNS Const ref to element.
        ///
        /// @EXPECTS {!isEmpty()}.
        ///     @THROWS OutOfRangeException: In Debug.
        ///
        /// @TIME_COMPLEXITY Constant.
        /// ----------------------------------------------------------------------------------------
        constexpr auto ElemFront() const -> const TElem&;

        /// ----------------------------------------------------------------------------------------
        /// Access last element.
        ///
        /// @RETURNS Const ref to element.
        ///
        /// @EXPECTS {!isEmpty()}.
        ///     @THROWS OutOfRangeException: In Debug.
        ///
        /// @TIME_COMPLEXITY Constant.
        /// ----------------------------------------------------------------------------------------
        constexpr auto ElemBack() const -> const TElem&;

        /// ----------------------------------------------------------------------------------------
        /// Pointer to underlying arr.
        ///
        /// @RETURNS Const ptr to underlying arr.
        ///
        /// @TIME_COMPLEXITY Constant.
        /// ----------------------------------------------------------------------------------------
        constexpr auto data() const -> const TElem*
        {
            return _Data();
        }

        /// ----------------------------------------------------------------------------------------
        /// Get count of elements.
        /// ----------------------------------------------------------------------------------------
        constexpr auto count() const -> usize
        {
            return _Count();
        }

        /// ----------------------------------------------------------------------------------------
        /// Is arr empty.
        /// ----------------------------------------------------------------------------------------
        constexpr auto isEmpty() const -> bool
        {
            return _Count() == 0;
        }

        ////
        ///-------------------------------------------------------------------------------------------
        //// Iteration
        ////
        ///-------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// {TIter} to the first element.
        /// ----------------------------------------------------------------------------------------
        constexpr auto iter() const -> TIter
        {
            return TIter{ _Data() };
        }

        /// ----------------------------------------------------------------------------------------
        /// {TIter} to element following the last element.
        /// ----------------------------------------------------------------------------------------
        constexpr auto iterEnd() const -> TIter
        {
            return TIter{ _Data() + _Count() };
        }

        ////
        ///-------------------------------------------------------------------------------------------
        //// Implementations
        ////
        ///-------------------------------------------------------------------------------------------

    protected:
        constexpr auto _ValidateIndexForAccess(isize index) const -> bool
        {
            return index > 0 && index < (isize)_Count();
        }

        using TImpl::_Count;
        using TImpl::_Data;
    };
}
