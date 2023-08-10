#pragma once
#include "_ConstArrImplHelper.decl.h"

namespace Atom
{
    template <typename TImpl>
    class _ArrImplHelper : public _ConstArrImplHelper<TImpl>
    {
    //// -------------------------------------------------------------------------------------------
    //// Aliases
    //// -------------------------------------------------------------------------------------------

    private:
        using Base = _ConstArrImplHelper<TImpl>;

    public:
        using TElem = typename Base::TElem;
        using TIter = typename Base::TIter;
        using TIterEnd = typename Base::TIterEnd;
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
        using Base::data;
        using Base::count;
        using Base::isEmpty;

        /// ----------------------------------------------------------------------------------------
        /// Access element at index `index`.
        /// 
        /// # Parameters
        /// - `index`: Index of element to access.
        /// 
        /// # Returns
        ///     Ref to element.
        /// 
        /// # Time Complexity
        ///     Constant
        /// ----------------------------------------------------------------------------------------
        constexpr auto ElemAt(usize index) -> TElem&;

        /// ----------------------------------------------------------------------------------------
        /// Access element at index `index`. Same as [`ElemAt(usize index)`], but doesn't perform 
        /// bounds checking in Release.
        /// 
        /// # Parameters
        /// - `index`: Index of element to access.
        /// 
        /// # Returns
        ///     Ref to element.
        /// 
        /// # Time Complexity
        ///     Constant
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator[](usize index) -> TElem&;

        /// ----------------------------------------------------------------------------------------
        /// Access first element.
        /// 
        /// # Returns
        ///     Ref to element.
        /// 
        /// # Time Complexity
        ///     Constant
        /// ----------------------------------------------------------------------------------------
        constexpr auto ElemFront() -> TElem&;

        /// ----------------------------------------------------------------------------------------
        /// Access last element.
        /// 
        /// # Returns
        ///     Ref to element.
        /// 
        /// # Time Complexity
        ///     Constant
        /// ----------------------------------------------------------------------------------------
        constexpr auto ElemBack() -> TElem&;

        /// ----------------------------------------------------------------------------------------
        /// Pointer to underlying arr.
        /// 
        /// # Returns
        ///     Ptr to underlying arr.
        /// 
        /// # Time Complexity
        ///     Constant
        /// ----------------------------------------------------------------------------------------
        constexpr auto data() -> TElem*
        {
            return _Data();
        }

    //// -------------------------------------------------------------------------------------------
    //// Iteration
    //// -------------------------------------------------------------------------------------------

        /// ----------------------------------------------------------------------------------------
        /// [`TMutIter`] to the first element.
        /// ----------------------------------------------------------------------------------------
        constexpr auto mutIter() -> TMutIter
        {
            return TMutIter{ _Data() };
        }

        /// ----------------------------------------------------------------------------------------
        /// [`TMutIter`] to element following the last element.
        /// ----------------------------------------------------------------------------------------
        constexpr auto mutIterEnd() -> TMutIter
        {
            return TMutIter{ _Data() + _Count() - 1 };
        }

    //// -------------------------------------------------------------------------------------------
    //// Implementations
    //// -------------------------------------------------------------------------------------------

    protected:
        using Base::_Data;
        using Base::_Count;
        using Base::_ValidateIndexForAccess;

        constexpr auto _Data() -> TElem*
        {
            return TImpl::_Data();
        }
    };
}