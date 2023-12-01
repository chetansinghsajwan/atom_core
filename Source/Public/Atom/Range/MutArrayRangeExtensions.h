#pragma once
#include "ArrayRangeExtensions.h"
#include "MutJumpRangeExtensions.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename _TConstRangeExtensionsImpl = void>
    class _MutArrayRangeExtensionsImpl:
        public _MutJumpRangeExtensionsImpl<TRange, _TConstRangeExtensionsImpl>
    {
        using Base = _MutJumpRangeExtensionsImpl<TRange, _TConstRangeExtensionsImpl>;

    protected:
        using _TImpl = typename Base::_TImpl;

    public:
        using TElem = typename Base::TElem;
        using TIter = typename Base::TIter;
        using TIterEnd = typename Base::TIterEnd;
        using TMutIter = typename Base::TMutIter;
        using TMutIterEnd = typename Base::TMutIterEnd;

    public:
        using Base::Base;
        using Base::operator=;

    public:
        constexpr auto mutData() -> MutMemPtr<TElem>
        {
            return _range().mutData();
        }

        constexpr auto count() const -> usize
        {
            return _range().count();
        }

        constexpr auto mutAt(usize i) -> TElem&
        {
            return (mutData() + i).getMut();
        }

        constexpr auto mutFront() -> TElem&
        {
            return mutAt(0);
        }

        constexpr auto mutBack() -> TElem&
        {
            Contracts::DebugExpects(count() > 0);

            return mutAt(count() - 1);
        }

        constexpr auto mutIter(usize i) -> TMutIter
        {
            return Base::mutIter().next(i);
        }

        using Base::mutIter;

    protected:
        using Base::_range;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    class _MutArrayRangeExtensionsImpl<TRange, void>:
        public _MutArrayRangeExtensionsImpl<TRange, _ArrayRangeExtensionsImpl<TRange>>
    {
        using Base = _MutArrayRangeExtensionsImpl<TRange, _ArrayRangeExtensionsImpl<TRange>>;

    public:
        using Base::Base;
        using Base::operator=;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename _TConstRangeExtensions = void>
    class MutArrayRangeExtensions: public MutJumpRangeExtensions<TRange, _TConstRangeExtensions>
    {
        using Base = MutJumpRangeExtensions<TRange, _TConstRangeExtensions>;

    protected:
        using _TImpl = typename Base::_TImpl;

    public:
        using TElem = typename Base::TElem;
        using TIter = typename Base::TIter;
        using TIterEnd = typename Base::TIterEnd;
        using TMutIter = typename Base::TMutIter;
        using TMutIterEnd = typename Base::TMutIterEnd;

    public:
        using Base::Base;
        using Base::operator=;

    public:
        ////////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// Access
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////////

        /// ----------------------------------------------------------------------------------------
        /// Get underlying ptr to arr.
        /// ----------------------------------------------------------------------------------------
        constexpr auto mutData() -> MutMemPtr<TElem>
        {
            return _impl().mutData();
        }

        /// ----------------------------------------------------------------------------------------
        /// Access element by index.
        ///
        /// # Parameters
        /// - `i`: Index of element to access.
        ///
        /// # Time Complexity
        /// Constant.
        /// ----------------------------------------------------------------------------------------
        constexpr auto mutAt(usize i) -> TElem&
        {
            Contracts::Expects(isIndexInRange(i), "Index is out of range.");

            return _impl().mutAt(i);
        }

        /// ----------------------------------------------------------------------------------------
        /// Access element by index.
        ///
        /// # Parameters
        /// - `i`: Index of element to access.
        ///
        /// # Time Complexity
        /// Constant.
        /// ----------------------------------------------------------------------------------------
        constexpr auto operator[](usize i) -> TElem&
        {
            Contracts::DebugExpects(isIndexInRange(i), "Index is out of range.");

            return _impl().mutAt(i);
        }

        using Base::operator[];

        /// ----------------------------------------------------------------------------------------
        /// Access first element.
        ///
        /// # Time Complexity
        /// Constant.
        /// ----------------------------------------------------------------------------------------
        constexpr auto mutFront() -> TElem&
        {
            Contracts::DebugExpects(not isEmpty(), "Range is empty.");

            return _impl().mutFront();
        }

        /// ----------------------------------------------------------------------------------------
        /// Access last element.
        ///
        /// # Time Complexity
        /// Constant.
        /// ----------------------------------------------------------------------------------------
        constexpr auto mutBack() -> TElem&
        {
            Contracts::DebugExpects(not isEmpty(), "Range is empty.");

            return _impl().mutBack();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// Iteration
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////////

        /// ----------------------------------------------------------------------------------------
        /// Get iter to element at index `i`.
        ///
        /// # Parameters
        ///
        /// - `i`: Index of the element to get iter at.
        /// ----------------------------------------------------------------------------------------
        constexpr auto mutIter(usize i) const -> TMutIter
        {
            Contracts::Expects(isIndexInRange(i), "Index is out of range.");

            return _impl().mutIter(i);
        }

        using Base::mutIter;

        ////////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// View
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////////

        /// ----------------------------------------------------------------------------------------
        /// # To Do: Review this.
        /// ----------------------------------------------------------------------------------------
        constexpr auto range(usize from, usize to) const {}

        /// ----------------------------------------------------------------------------------------
        /// # To Do: Review this.
        /// ----------------------------------------------------------------------------------------
        constexpr auto rangeFrom(usize from) const {}

        /// ----------------------------------------------------------------------------------------
        /// # To Do: Review this.
        /// ----------------------------------------------------------------------------------------
        constexpr auto rangeTo(usize to) const {}

        using Base::isEmpty;
        using Base::isIndexInRange;

    protected:
        using Base::_impl;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    class MutArrayRangeExtensions<TRange, void>:
        public MutArrayRangeExtensions<TRange,
            ArrayRangeExtensions<TRange, _MutArrayRangeExtensionsImpl<TRange>>>
    {
        using Base = MutArrayRangeExtensions<TRange,
            ArrayRangeExtensions<TRange, _MutArrayRangeExtensionsImpl<TRange>>>;

    public:
        using Base::Base;
        using Base::operator=;
    };
}
