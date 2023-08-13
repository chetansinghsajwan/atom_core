#pragma once
#include "ArrRangeTrait.h"
#include "MutRangeTrait.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// User defined implementation for [`MutArrRangeTrait`].
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    class MutArrRangeTraitImpl;

    /// --------------------------------------------------------------------------------------------
    /// Requirements for [`MutArrRangeTraitImpl`].
    /// 
    /// # To Do: Implement this.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    concept RMutArrRangeTraitImpl = RDefaultConstructible<MutArrRangeTraitImpl<TRange>>;

    /// --------------------------------------------------------------------------------------------
    /// Implementation of [`MutArrRangeTrait`].
    /// 
    /// # To Do
    /// 
    /// - Add `iter()` and `iterEnd()` functions.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    class _MutArrRangeTraitImpl:
        private MutArrRangeTraitImpl<TRange>
    {
        using _Impl = MutArrRangeTraitImpl<TRange>;

    public:
        using TElem = typename _Impl::TElem;
        using TIter = ArrIter<TElem>;
        using TIterEnd = TIter;
        using TMutIter = MutArrIter<TElem>;
        using TMutIterEnd = TMutIter;

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto getData() -> TElem*
        {
            return _Impl::getData();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto getData() const -> const TElem*
        {
            return _Impl::getData();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto getCount() const -> usize
        {
            return _Impl::getCount();
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// Trait for [ArrRange].
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    class MutArrRangeTrait:
        public ArrRangeTrait<TRange>,
        public MutRangeTrait<TRange>,
        private _MutArrRangeTraitImpl<TRange>
    {
        using _Impl = _MutArrRangeTraitImpl<TRange>;

    public:
        using TElem = typename _Impl::TElem;
        using TIter = ArrIter<TElem>;
        using TIterEnd = TIter;
        using TMutIter = MutArrIter<TElem>;
        using TMutIterEnd = TIter;

    //// -------------------------------------------------------------------------------------------
    //// Access
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// Get underlying ptr to arr.
        /// ----------------------------------------------------------------------------------------
        constexpr auto data() -> TElem*
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
        constexpr auto at(usize i) -> TElem&
        {
            expects(isIndexInRange(i), "Index is out of range.");

            return _at(i);
        }

        using ArrRangeTrait<TRange>::operator[];

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
            debug_expects(isIndexInRange(i), "Index is out of range.");

            return _at(i);
        }

        /// ----------------------------------------------------------------------------------------
        /// Access first element.
        ///
        /// # Time Complexity
        /// Constant.
        /// ----------------------------------------------------------------------------------------
        constexpr auto front() -> TElem&
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
        constexpr auto back() -> TElem&
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
        constexpr auto mutIter(usize i) const -> TMutIterEnd
        {
            expects(isIndexInRange(i), "Index is out of range.");
    
            return _iter() + i;
        }

        using MutRangeTrait<TRange>::mutIter;

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
        constexpr auto _data() -> TElem*
        {
            return _Impl::getData();
        }

        constexpr auto _count() const -> usize
        {
            return _Impl::getCount();
        }

        constexpr auto _at(usize i) -> TElem&
        {
            return _data()[i];
        }

        constexpr auto _front() -> TElem&
        {
            return _at(0);
        }

        constexpr auto _back() -> TElem&
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
    /// [`ArrRangeTraitImpl`] using [`MutArrRangeTraitImpl`].
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    requires(RMutArrRangeTraitImpl<TRange>)
        and (not RDerivedFrom<MutArrRangeTraitImpl<TRange>, _DontInheritRangeImpl>)
    class ArrRangeTraitImpl<TRange>:
        private MutArrRangeTraitImpl<TRange>
    {
        using _Impl = MutArrRangeTraitImpl<TRange>;

    public:
        using TElem = typename _Impl::TElem;

    public:
        constexpr auto getData() const -> const TElem*
        {
            return _Impl::getData();
        }

        constexpr auto getCount() const -> usize
        {
            return _Impl::getCount();
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// [`MutRangeTraitImpl`] using [`MutArrRangeTraitImpl`].
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    requires(RMutArrRangeTraitImpl<TRange>)
    class MutRangeTraitImpl<TRange>:
        private MutArrRangeTraitImpl<TRange>,
        public _DontInheritRangeImpl
    {
        using _Impl = MutArrRangeTraitImpl<TRange>;

    public:
        using TElem = typename _Impl::TElem;
        using TIter = ArrIter<TElem>;
        using TIterEnd = TIter;
        using TMutIter = MutArrIter<TElem>;
        using TMutIterEnd = TMutIter;

    public:
        struct DontInheritFromRange{};

    public:
        constexpr auto iter() const -> TIter
        {
            return TIter{ _Impl::getData() };
        }

        constexpr auto iterEnd() const -> TIterEnd
        {
            return TIter{ _Impl::getData() + _Impl::getCount() };
        }

        constexpr auto mutIter() const -> TIter
        {
            return TMutIter{ _Impl::getData() };
        }

        constexpr auto mutIterEnd() const -> TIterEnd
        {
            return TMutIter{ _Impl::getData() + _Impl::getCount() };
        }
    };
}
