#pragma once
#include "_ConstArrImplHelper.decl.h"

namespace Atom
{
    template <tname TImpl>
    class _ArrImplHelper: public _ConstArrImplHelper<TImpl>
    {
    //// -------------------------------------------------------------------------------------------
    //// Aliases
    //// -------------------------------------------------------------------------------------------

        using Base = _ConstArrImplHelper<TImpl>;

    public:
        using TElem = tname Base::TElem;
        using TIter = tname Base::TIter;
        using TIterEnd = tname Base::TIterEnd;
        using TMutIter = MutArrIter<TElem>;
        using TMutIterEnd = TMutIter;

    //// -------------------------------------------------------------------------------------------
    //// Access
    //// -------------------------------------------------------------------------------------------

    public:
        using Base::ElemAt;
        using Base::operator[];
        using Base::ElemFront;
        using Base::ElemBack;
        using Base::Data;
        using Base::Count;
        using Base::IsEmpty;

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
        constexpr fn ElemAt(usize index) -> TElem&;

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
        constexpr fn operator [](usize index) noex -> TElem&;

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
        constexpr fn ElemFront() noex -> TElem&;

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
        constexpr fn ElemBack() noex -> TElem&;

        /// ----------------------------------------------------------------------------------------
        /// Pointer to underlying arr.
        /// 
        /// @RETURNS Ptr to underlying arr.
        /// 
        /// @TIME_COMPLEXITY Constant.
        /// ----------------------------------------------------------------------------------------
        constexpr fn Data() noex -> TElem*
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
        constexpr fn MutIter() noex -> TMutIter
        {
            return TMutIter{ _Data() };
        }

        /// ----------------------------------------------------------------------------------------
        /// {TMutIter} to element following the last element.
        /// ----------------------------------------------------------------------------------------
        constexpr fn MutIterEnd() noex -> TMutIter
        {
            return TMutIter{ _Data() + _Count() - 1 };
        }

    //// -------------------------------------------------------------------------------------------
    //// Implementations
    //// -------------------------------------------------------------------------------------------

    public:
        using Base::_Data;
        using Base::_Count;
        using Base::_ValidateIndexForAccess;

        constexpr fn _Data() noex -> TElem*
        {
            return TImpl::_Data();
        }
    };
}