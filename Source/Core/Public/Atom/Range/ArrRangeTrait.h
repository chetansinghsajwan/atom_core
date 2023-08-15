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
        constexpr auto getCount() const -> usize
        {
            return _Impl::getCount();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto getData() const -> const TElem*
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
        constexpr auto data() const -> const TElem*
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
        constexpr auto at(usize i) const -> const TElem&
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
        constexpr auto operator[](usize i) const -> const TElem&
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
        constexpr auto front() const -> const TElem&
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
        constexpr auto back() const -> const TElem&
        {
            debug_expects(not isEmpty(), "Range is empty.");

            return _back();
        }

        /// ----------------------------------------------------------------------------------------
        /// Get count of elements.
        /// ----------------------------------------------------------------------------------------
        constexpr auto count() const -> usize
        {
            return _count();
        }

        /// ----------------------------------------------------------------------------------------
        /// Is range empty.
        /// ----------------------------------------------------------------------------------------
        constexpr auto isEmpty() const -> bool
        {
            return _count() == 0;
        }

    //// -------------------------------------------------------------------------------------------
    //// Iteration
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// Get iter to element at index `i`.
        ///
        /// # Parameters
        ///
        /// - `i`: Index of the element to get iter at.
        /// ----------------------------------------------------------------------------------------
        constexpr auto iter(usize i) const -> TIter
        {
            expects(isIndexInRange(i), "Index is out of range.");

            return _iter().next(i);
        }

        using RangeTrait<TRange>::iter;

    //// -------------------------------------------------------------------------------------------
    //// View
    //// -------------------------------------------------------------------------------------------

    public:
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

    //// -------------------------------------------------------------------------------------------
    //// Validation
    //// -------------------------------------------------------------------------------------------

    public:
        constexpr auto isIndexInRange(usize i) const -> bool
        {
            return i < _count();
        }

    //// -------------------------------------------------------------------------------------------
    //// Implementations
    //// -------------------------------------------------------------------------------------------

    private:
        constexpr auto _data() const -> const TElem*
        {
            return _Impl::getData();
        }

        constexpr auto _count() const -> usize
        {
            return _Impl::getCount();
        }

        constexpr auto _at(usize i) const -> const TElem&
        {
            return _data()[i];
        }

        constexpr auto _front() const -> const TElem&
        {
            return _at(0);
        }

        constexpr auto _back() const -> const TElem&
        {
            debug_expects(_count() > 0);

            return _at(_count() - 1);
        }

        constexpr auto _iter() const -> TIter
        {
            return TIter{ _data() };
        }

        constexpr auto _iterEnd() const -> TIterEnd
        {
            return TIter{ _data() + _count() };
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// [`RangeTraitImpl`] using [`ArrRangeTraitImpl`].
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    requires(RDefaultConstructible<ArrRangeTraitImpl<TRange>>)
    class RangeTraitImpl<TRange>:
        private ArrRangeTraitImpl<TRange>
    {
        using _Impl = ArrRangeTraitImpl<TRange>;

    public:
        using TElem = typename _Impl::TElem;
        using TIter = ArrIter<TElem>;
        using TIterEnd = TIter;

    public:
        constexpr auto iter() const -> TIter
        {
            return TIter{ _Impl::getData() };
        }

        constexpr auto iterEnd() const -> TIterEnd
        {
            return TIterEnd{ _Impl::getData() + _Impl::getCount() };
        }
    };
}
