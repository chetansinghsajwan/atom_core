#pragma once
#include "JumpRangeTrait.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// User defined implementation for [`ArrayRangeTrait`].
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    class ArrayRangeTraitImpl;

    /// --------------------------------------------------------------------------------------------
    /// Implementation of [`ArrayRangeTrait`].
    /// --------------------------------------------------------------------------------------------
    template <typename TRangeImpl>
    class _ArrayRangeTraitImpl: public _RangeTraitImpl<TRangeImpl>
    {
        using Base = _RangeTraitImpl<TRangeImpl>;

    public:
        using TImpl = typename Base::TImpl;
        using TElem = typename Base::TElem;
        using TIter = typename Base::TIter;
        using TIterEnd = typename Base::TIterEnd;

    public:
        constexpr auto data() const -> const TElem*
        {
            return _impl.data();
        }

        constexpr auto count() const -> usize
        {
            return _impl.count();
        }

        constexpr auto at(usize i) const -> const TElem&
        {
            debug_expects(isIndexInRange(i));

            return data()[i.val()];
        }

        constexpr auto front() const -> const TElem&
        {
            return at(0);
        }

        constexpr auto back() const -> const TElem&
        {
            return back(count() - 1);
        }

        constexpr auto isIndexInRange(usize i) const -> bool
        {
            return i < count();
        }

        constexpr auto iter(usize i) const -> TIter
        {
            return Base::iter().next(i);
        }

        using Base::iter;

    public:
        using Base::_impl;
    };

    /// --------------------------------------------------------------------------------------------
    /// Trait for [ArrayRange].
    /// --------------------------------------------------------------------------------------------
    template <typename TRangeImpl>
    class ArrayRangeTraitRecursive: public JumpRangeTraitRecursive<TRangeImpl>
    {
        using Base = JumpRangeTraitRecursive<TRangeImpl>;

    public:
        using TImpl = typename Base::TImpl;
        using TElem = typename Base::TElem;
        using TIter = typename Base::TIter;
        using TIterEnd = typename Base::TIterEnd;

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// Access
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

    public:
        /// ----------------------------------------------------------------------------------------
        /// Get underlying ptr to arr.
        /// ----------------------------------------------------------------------------------------
        constexpr auto data() const -> const TElem*
        {
            return _impl.data();
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

            return _impl.at(i);
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

            return _impl.at(i);
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

            return _impl.front();
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

            return _impl.back();
        }

        /// ----------------------------------------------------------------------------------------
        /// Get count of elements.
        /// ----------------------------------------------------------------------------------------
        constexpr auto count() const -> usize
        {
            return _impl.count();
        }

        /// ----------------------------------------------------------------------------------------
        /// Is range empty.
        /// ----------------------------------------------------------------------------------------
        constexpr auto isEmpty() const -> bool
        {
            return _impl.count() == 0;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// Iteration
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

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

            return _impl.iter(i);
        }

        using Base::iter;

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// View
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

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

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// Validation
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

    public:
        constexpr auto isIndexInRange(usize i) const -> bool
        {
            return _impl.isIndexInRange(i);
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// Fields
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

    public:
        using Base::_impl;
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    class ArrayRangeTrait: public ArrayRangeTraitRecursive<
        _ArrayRangeTraitImpl<ArrayRangeTraitImpl<TRange>>>
    {};
}
