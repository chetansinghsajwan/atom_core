#pragma once
#include "_ConstArrImplHelper.decl.h"

namespace Atom
{
    template <tname TImpl>
    class _ArrImplHelper extends _ConstArrImplHelper<TImpl>
    {
    //// -------------------------------------------------------------------------------------------
    //// Aliases
    //// -------------------------------------------------------------------------------------------

        priv using Base = _ConstArrImplHelper<TImpl>;

        pub using TElem = tname Base::TElem;
        pub using TIter = tname Base::TIter;
        pub using TIterEnd = tname Base::TIterEnd;
        pub using TMutIter = MutArrIter<TElem>;
        pub using TMutIterEnd = TMutIter;

    //// -------------------------------------------------------------------------------------------
    //// Access
    //// -------------------------------------------------------------------------------------------

        pub using Base::ElemAt;
        pub using Base::operator[];
        pub using Base::ElemFront;
        pub using Base::ElemBack;
        pub using Base::Data;
        pub using Base::Count;
        pub using Base::IsEmpty;

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
        pub cexpr fn operator [](usize index) noex -> TElem&;

        /// ----------------------------------------------------------------------------------------
        /// Access first element.
        /// 
        /// # Returns
        ///     Ref to element.
        /// 
        /// # Time Complexity
        ///     Constant
        /// ----------------------------------------------------------------------------------------
        pub cexpr fn ElemFront() noex -> TElem&;

        /// ----------------------------------------------------------------------------------------
        /// Access last element.
        /// 
        /// # Returns
        ///     Ref to element.
        /// 
        /// # Time Complexity
        ///     Constant
        /// ----------------------------------------------------------------------------------------
        pub cexpr fn ElemBack() noex -> TElem&;

        /// ----------------------------------------------------------------------------------------
        /// Pointer to underlying arr.
        /// 
        /// # Returns
        ///     Ptr to underlying arr.
        /// 
        /// # Time Complexity
        ///     Constant
        /// ----------------------------------------------------------------------------------------
        pub cexpr fn Data() noex -> TElem*
        {
            return _Data();
        }

    //// -------------------------------------------------------------------------------------------
    //// Iteration
    //// -------------------------------------------------------------------------------------------

        /// ----------------------------------------------------------------------------------------
        /// [`TMutIter`] to the first element.
        /// ----------------------------------------------------------------------------------------
        pub cexpr fn MutIter() noex -> TMutIter
        {
            return TMutIter{ _Data() };
        }

        /// ----------------------------------------------------------------------------------------
        /// [`TMutIter`] to element following the last element.
        /// ----------------------------------------------------------------------------------------
        pub cexpr fn MutIterEnd() noex -> TMutIter
        {
            return TMutIter{ _Data() + _Count() - 1 };
        }

    //// -------------------------------------------------------------------------------------------
    //// Implementations
    //// -------------------------------------------------------------------------------------------

        prot using Base::_Data;
        prot using Base::_Count;
        prot using Base::_ValidateIndexForAccess;

        prot cexpr fn _Data() noex -> TElem*
        {
            return TImpl::_Data();
        }
    };
}