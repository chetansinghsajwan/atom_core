#pragma once
#include "_ConstArrImplHelper.decl.h"

namespace Atom
{
    template <typename _TImpl>
    class _ArrImplHelper: public _ConstArrImplHelper<_TImpl>
    {
    //// -------------------------------------------------------------------------------------------
    //// Aliases
    //// -------------------------------------------------------------------------------------------

        using _TBase = _ConstArrImplHelper<_TImpl>;

    public:
        using TElem = typename _TBase::TElem;
        using TIter = typename _TBase::TIter;
        using TIterEnd = typename _TBase::TIterEnd;
        using TMutIter = MutArrIter<TElem>;
        using TMutIterEnd = TMutIter;

    //// -------------------------------------------------------------------------------------------
    //// Access
    //// -------------------------------------------------------------------------------------------

    public:
        using _TBase::ElemAt;
        using _TBase::operator[];
        using _TBase::ElemFront;
        using _TBase::ElemBack;
        using _TBase::Data;
        using _TBase::Count;
        using _TBase::IsEmpty;

        /// ----------------------------------------------------------------------------------------
        /// Access element at index{index}.
        /// 
        /// @PARAM index: Index of element to access.
        /// 
        /// @RETURNS Ref to element.
        /// 
        /// @EXPECTS {index} is in range[0, Count() - 1].
        ///     @THROWS IndexOutOfRangeException.
        /// 
        /// @TIME_COMPLEXITY Constant.
        /// ----------------------------------------------------------------------------------------
        constexpr auto ElemAt(usize index)
            -> TElem&;

        /// ----------------------------------------------------------------------------------------
        /// Access element at index{index}. Same as {ElemAt(...)}, but doesn't perform bounds checking
        /// in Release.
        /// 
        /// @PARAM index: Index of element to access.
        /// 
        /// @RETURNS Ref to element.
        /// 
        /// @EXPECTS {index} is in range[0, Count() - 1].
        ///     @THROWS IndexOutOfRangeException: In Debug.
        /// 
        /// @TIME_COMPLEXITY Constant.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator [](usize index) noexcept
            -> TElem&;

        /// ----------------------------------------------------------------------------------------
        /// Access first element.
        /// 
        /// @RETURNS Ref to element.
        /// 
        /// @EXPECTS {!IsEmpty()}.
        ///     @THROWS OutOfRangeException: In Debug.
        /// 
        /// @TIME_COMPLEXITY Constant.
        /// ----------------------------------------------------------------------------------------
        constexpr auto ElemFront() noexcept
            -> TElem&;

        /// ----------------------------------------------------------------------------------------
        /// Access last element.
        /// 
        /// @RETURNS Ref to element.
        /// 
        /// @EXPECTS {!IsEmpty()}.
        ///     @THROWS OutOfRangeException: In Debug.
        /// 
        /// @TIME_COMPLEXITY Constant.
        /// ----------------------------------------------------------------------------------------
        constexpr auto ElemBack() noexcept
            -> TElem&;

        /// ----------------------------------------------------------------------------------------
        /// Pointer to underlying arr.
        /// 
        /// @RETURNS Ptr to underlying arr.
        /// 
        /// @TIME_COMPLEXITY Constant.
        /// ----------------------------------------------------------------------------------------
        constexpr auto Data() noexcept
            -> TElem*
        {
            return _Data();
        }

    //// -------------------------------------------------------------------------------------------
    //// Iteration
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// {TMutIter} to the first element.
        /// ----------------------------------------------------------------------------------------
        constexpr auto MutIter() noexcept
            -> TMutIter
        {
            return TMutIter{ _Data() };
        }

        /// ----------------------------------------------------------------------------------------
        /// {TMutIter} to element following the last element.
        /// ----------------------------------------------------------------------------------------
        constexpr auto MutIterEnd() noexcept
            -> TMutIter
        {
            return TMutIter{ _Data() + _Count() - 1 };
        }

    //// -------------------------------------------------------------------------------------------
    //// Implementations
    //// -------------------------------------------------------------------------------------------

    public:
        using _TBase::_Data;
        using _TBase::_Count;
        using _TBase::_ValidateIndexForAccess;

        constexpr auto _Data() noexcept
            -> TElem*
        {
            return _TImpl::_Data();
        }
    };
}