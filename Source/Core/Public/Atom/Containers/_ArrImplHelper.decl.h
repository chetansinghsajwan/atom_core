#pragma once
#include "_ConstArrImplHelper.decl.h"

namespace Atom
{
    template <typename TImpl>
    class _ArrImplHelper extends _ConstArrImplHelper<TImpl>
    {
    //// -------------------------------------------------------------------------------------------
    //// Aliases
    //// -------------------------------------------------------------------------------------------

        priv using Base = _ConstArrImplHelper<TImpl>;

        pub using TElem = typename Base::TElem;
        pub using TIter = typename Base::TIter;
        pub using TIterEnd = typename Base::TIterEnd;
        pub using TMutIter = MutArrIter<TElem>;
        pub using TMutIterEnd = TMutIter;

    //// -------------------------------------------------------------------------------------------
    //// Access
    //// -------------------------------------------------------------------------------------------

        pub using Base::ElemAt;
        pub using Base::op[];
        pub using Base::ElemFront;
        pub using Base::ElemBack;
        pub using Base::data;
        pub using Base::count;
        pub using Base::isEmpty;

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
        pub cexpr fn ElemAt(usize index) -> TElem&;

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
        pub cexpr fn op[](usize index) -> TElem&;

        /// ----------------------------------------------------------------------------------------
        /// Access first element.
        /// 
        /// # Returns
        ///     Ref to element.
        /// 
        /// # Time Complexity
        ///     Constant
        /// ----------------------------------------------------------------------------------------
        pub cexpr fn ElemFront() -> TElem&;

        /// ----------------------------------------------------------------------------------------
        /// Access last element.
        /// 
        /// # Returns
        ///     Ref to element.
        /// 
        /// # Time Complexity
        ///     Constant
        /// ----------------------------------------------------------------------------------------
        pub cexpr fn ElemBack() -> TElem&;

        /// ----------------------------------------------------------------------------------------
        /// Pointer to underlying arr.
        /// 
        /// # Returns
        ///     Ptr to underlying arr.
        /// 
        /// # Time Complexity
        ///     Constant
        /// ----------------------------------------------------------------------------------------
        pub cexpr fn data() -> TElem*
        {
            return _Data();
        }

    //// -------------------------------------------------------------------------------------------
    //// Iteration
    //// -------------------------------------------------------------------------------------------

        /// ----------------------------------------------------------------------------------------
        /// [`TMutIter`] to the first element.
        /// ----------------------------------------------------------------------------------------
        pub cexpr fn mutIter() -> TMutIter
        {
            return TMutIter{ _Data() };
        }

        /// ----------------------------------------------------------------------------------------
        /// [`TMutIter`] to element following the last element.
        /// ----------------------------------------------------------------------------------------
        pub cexpr fn mutIterEnd() -> TMutIter
        {
            return TMutIter{ _Data() + _Count() - 1 };
        }

    //// -------------------------------------------------------------------------------------------
    //// Implementations
    //// -------------------------------------------------------------------------------------------

        prot using Base::_Data;
        prot using Base::_Count;
        prot using Base::_ValidateIndexForAccess;

        prot cexpr fn _Data() -> TElem*
        {
            return TImpl::_Data();
        }
    };
}