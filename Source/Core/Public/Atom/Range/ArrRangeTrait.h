#pragma once
#include "RangeTrait.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// User defined implementation for [`ArrRangeTrait`].
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    class ArrRangeTraitImpl;

    /// --------------------------------------------------------------------------------------------
    /// Implementation of [`ArrRangeTrait`].
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    class _ArrRangeTraitImpl:
        private ArrRangeTraitImpl<TRange>
    {
        using _Impl = ArrRangeTraitImpl<TRange>;

    public:
        using TElem = typename _Impl::TElem;
        using TIter = ArrIter<TElem>;
        using TIterEnd = TIter;

    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr fn getCount() const -> usize
        {
            return _Impl::getCount();
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr fn getData() const -> const TElem*
        {
            return _Impl::getData();
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// Trait for [ArrRange].
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    class ArrRangeTrait:
        public RangeTrait<TRange>,
        private _ArrRangeTraitImpl<TRange>
    {
        using _Impl = _ArrRangeTraitImpl<TRange>;

    public:
        using TElem = typename _Impl::TElem;
        using TIter = ArrIter<TElem>;
        using TIterEnd = TIter;

    //// -------------------------------------------------------------------------------------------
    //// Access
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// Get underlying ptr to arr.
        /// ----------------------------------------------------------------------------------------
        constexpr fn data() const -> const TElem*
        {
            return _data();
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
        constexpr fn at(usize i) const -> const TElem&
        {
            expects(isIndexInRange(i), "Index is out of range.");

            return _at(i);
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
        constexpr fn operator[](usize i) const -> const TElem&
        {
            debug_expects(isIndexInRange(i), "Index is out of range.");

            return _at(i);
        }

        /// ----------------------------------------------------------------------------------------
        /// Access first element.
        /// 
        /// # Time Complexity
        /// Constant.
        /// ----------------------------------------------------------------------------------------
        constexpr fn front() const -> const TElem&
        {
            debug_expects(not isEmpty(), "Range is empty.");

            return _front();
        }

        /// ----------------------------------------------------------------------------------------
        /// Access last element.
        /// 
        /// # Time Complexity
        /// Constant.
        /// ----------------------------------------------------------------------------------------
        constexpr fn back() const -> const TElem&
        {
            debug_expects(not isEmpty(), "Range is empty.");

            return _back();
        }

        /// ----------------------------------------------------------------------------------------
        /// Get count of elements.
        /// ----------------------------------------------------------------------------------------
        constexpr fn count() const -> usize
        {
            return _count();
        }

        /// ----------------------------------------------------------------------------------------
        /// Is range empty.
        /// ----------------------------------------------------------------------------------------
        constexpr fn isEmpty() const -> bool
        {
            return _count() == 0;
        }

    //// -------------------------------------------------------------------------------------------
    //// Iteration
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr fn iter() const -> TIter
        {
            return _iter();
        }

        /// ----------------------------------------------------------------------------------------
        /// Get iter to element at index `i`.
        /// 
        /// # Parameters
        /// 
        /// - `i`: Index of the element to get iter at.
        /// ----------------------------------------------------------------------------------------
        constexpr fn iter(usize i) const -> TIter
        {
            debug_expects(isIndexInRange(i), "Index is out of range.");

            return _iter() + i;
        }

    //// -------------------------------------------------------------------------------------------
    //// View
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// # To Do: Review this.
        /// ----------------------------------------------------------------------------------------
        constexpr fn range(usize from, usize to) const
        {
        }

        /// ----------------------------------------------------------------------------------------
        /// # To Do: Review this.
        /// ----------------------------------------------------------------------------------------
        constexpr fn rangeFrom(usize from) const
        {
        }

        /// ----------------------------------------------------------------------------------------
        /// # To Do: Review this.
        /// ----------------------------------------------------------------------------------------
        constexpr fn rangeTo(usize to) const
        {
        }

    //// -------------------------------------------------------------------------------------------
    //// Validation
    //// -------------------------------------------------------------------------------------------

    public:
        constexpr fn isIndexInRange(usize i) const -> bool
        {
            return i < _count();
        }

    //// -------------------------------------------------------------------------------------------
    //// Implementations
    //// -------------------------------------------------------------------------------------------

    private:
        constexpr fn _data() const -> const TElem*
        {
            return _Impl::getData();
        }

        constexpr fn _count() const -> usize
        {
            return _Impl::getCount();
        }

        constexpr fn _at(usize i) const -> const TElem&
        {
            return _data()[i];
        }

        constexpr fn _front() const -> const TElem&
        {
            return _at(0);
        }

        constexpr fn _back() const -> const TElem&
        {
            debug_expects(_count() > 0);

            return _at(_count() - 1);
        }

        constexpr fn _iter() const -> TIter
        {
            return TIter{ _data() };
        }

        constexpr fn _iterEnd() const -> TIterEnd
        {
            return TIter{ _data() + _count() };
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// [`RangeTraitImpl`] using [`ArrRangeTraitImpl`].
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    requires(RDefaultConstructible<ArrRangeTraitImpl<TRange>>)
    class RangeTraitImpl<TRange>: private ArrRangeTraitImpl<TRange>
    {
        using _Impl = ArrRangeTraitImpl<TRange>;

    public:
        using TElem = typename _Impl::TElem;
        using TIter = ArrIter<TElem>;
        using TIterEnd = TIter;

    public:
        constexpr fn iter() const -> TIter
        {
            return TIter{ _Impl::getData() };
        }

        constexpr fn iterEnd() const -> TIterEnd
        {
            return TIterEnd{ _Impl::getData() + _Impl::getCount() };
        }
    };
}