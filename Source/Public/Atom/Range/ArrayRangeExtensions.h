#pragma once
#include "JumpRangeExtensions.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// Implementation of [`ArrayRangeExtensions`].
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    class _ArrayRangeExtensionsImpl: public _JumpRangeExtensionsImpl<TRange>
    {
        using Base = _JumpRangeExtensionsImpl<TRange>;

    protected:
        using _TImpl = typename Base::_TImpl;

    public:
        using TElem = typename Base::TElem;
        using TIter = typename Base::TIter;
        using TIterEnd = typename Base::TIterEnd;

    public:
        using Base::Base;
        using Base::operator=;

    public:
        constexpr auto data() const -> const TElem*
        {
            return _range().data();
        }

        constexpr auto count() const -> usize
        {
            return _range().count();
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

    protected:
        using Base::_range;
    };

    /// --------------------------------------------------------------------------------------------
    /// [ArrayRangeExtensions].
    /// --------------------------------------------------------------------------------------------
    template <typename TRange, typename _TRangeExtensionsImpl_ = void>
    class ArrayRangeExtensions: public JumpRangeExtensions<TRange, _TRangeExtensionsImpl_>
    {
        using Base = JumpRangeExtensions<TRange, _TRangeExtensionsImpl_>;

    protected:
        using _TImpl = typename Base::_TImpl;

    public:
        using TElem = typename Base::TElem;
        using TIter = typename Base::TIter;
        using TIterEnd = typename Base::TIterEnd;

    public:
        using Base::Base;
        using Base::operator=;

    public:
        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// Access
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

        /// ----------------------------------------------------------------------------------------
        /// Get underlying ptr to arr.
        /// ----------------------------------------------------------------------------------------
        constexpr auto data() const -> const TElem*
        {
            return _impl().data();
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

            return _impl().at(i);
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

            return _impl().at(i);
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

            return _impl().front();
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

            return _impl().back();
        }

        /// ----------------------------------------------------------------------------------------
        /// Get count of elements.
        /// ----------------------------------------------------------------------------------------
        constexpr auto count() const -> usize
        {
            return _impl().count();
        }

        /// ----------------------------------------------------------------------------------------
        /// Is range empty.
        /// ----------------------------------------------------------------------------------------
        constexpr auto isEmpty() const -> bool
        {
            return _impl().count() == 0;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// Iteration
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

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

            return _impl().iter(i);
        }

        using Base::iter;

        ////////////////////////////////////////////////////////////////////////////////////////////
        ////
        //// View
        ////
        ////////////////////////////////////////////////////////////////////////////////////////////

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

        constexpr auto isIndexInRange(usize i) const -> bool
        {
            return _impl().isIndexInRange(i);
        }

    protected:
        using Base::_impl;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    class ArrayRangeExtensions<TRange, void>:
        public ArrayRangeExtensions<TRange, _ArrayRangeExtensionsImpl<TRange>>
    {
        using Base = ArrayRangeExtensions<TRange, _ArrayRangeExtensionsImpl<TRange>>;

    public:
        using Base::Base;
        using Base::operator=;
    };
}
