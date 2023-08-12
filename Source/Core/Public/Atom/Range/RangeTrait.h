#pragma once
#include "ArrIter.h"
#include "RangeReq.h"
#include "StdIterWrapForAtomIter.h"

#include <algorithm>

namespace Atom
{
    template <typename TRange>
    class RangeTraitImpl;

    template <typename TRange>
    class _RangeTraitImpl:
        private RangeTraitImpl<TRange>
    {
        using _Impl = RangeTraitImpl<TRange>;

    public:
        using TElem = typename _Impl::TElem;
        using TIter = typename _Impl::TIter;
        using TIterEnd = typename _Impl::TIterEnd;

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto iter() const -> TIter
        {
            return _Impl::iter();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto iterEnd() const -> TIterEnd
        {
            return _Impl::iterEnd();
        }

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TElem1>
        constexpr auto findElem(const TElem1& el) -> TIter
        {
            StdIterWrapForAtomIter stdIter{ iter() };
            StdIterWrapForAtomIter stdIterEnd{ iterEnd() };
            return std::find(stdIter, stdIterEnd, el).iter;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TRange1>
        constexpr auto findRange(const TRange1& range) -> TIter
        {
            StdIterWrapForAtomIter stdIter1{ iter() };
            StdIterWrapForAtomIter stdIterEnd1{ iterEnd() };
            StdIterWrapForAtomIter stdIter2{ range.iter() };
            StdIterWrapForAtomIter stdIterEnd2{ range.iterEnd() };
            return std::find(stdIter1, stdIterEnd1, stdIter2, stdIterEnd2);
        }

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TRange1>
        auto compare(const TRange1& range) -> i8
        {
            StdIterWrapForAtomIter stdIter1{ iter() };
            StdIterWrapForAtomIter stdIterEnd1{ iterEnd() };
            StdIterWrapForAtomIter stdIter2{ range.iter() };
            StdIterWrapForAtomIter stdIterEnd2{ range.iterEnd() };

            return std::equal(stdIter1, stdIterEnd1, stdIter2, stdIterEnd2);
        }

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto getCount() const -> usize
        {
            if constexpr (RJumpIterPair<TIter, TIterEnd>)
            {
                return iterEnd() - iter();
            }

            usize count = 0;
            for (auto it = iter(); it != iterEnd(); it++)
                count++;

            return count;
        }
    };

    template <typename TRange>
    class RangeTrait:
        private _RangeTraitImpl<TRange>
    {
        /// ----------------------------------------------------------------------------------------
        /// # To Do: Should this alias be used for public API.
        /// ----------------------------------------------------------------------------------------
        using TraitImpl = RangeTraitImpl<TRange>;
        using _Impl = _RangeTraitImpl<TRange>;

    public:
        using TElem = typename TraitImpl::TElem;
        using TIter = typename TraitImpl::TIter;
        using TIterEnd = typename TraitImpl::TIterEnd;

    //// -------------------------------------------------------------------------------------------
    //// Iteration
    //// -------------------------------------------------------------------------------------------

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto iter() const -> TIter
        {
            return _Impl::iter();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto iterEnd() const -> TIterEnd
        {
            return _Impl::iterEnd();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto begin() const -> TIter
        {
            return _Impl::iter();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto end() const -> TIterEnd
        {
            return _Impl::iterEnd();
        }

    //// -------------------------------------------------------------------------------------------
    //// Search
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TElem1>
        constexpr auto find(const TElem1& el) -> TIter
            requires(REqualityComparableWith<TElem, TElem1>)
        {
            return _Impl::findElem(el);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TRange1>
        constexpr auto findRange(const TRange1& range) -> TIter
            requires(RFwdRange<TRange1>)
                    and (REqualityComparableWith<TElem, typename TRange1::TElem>)
        {
            return _Impl::findRange(range);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TElem1>
        constexpr auto contains(const TElem1& el) -> bool
            requires(REqualityComparableWith<TElem, TElem1>)
        {
            return _Impl::findElem(el) != _Impl::iterEnd();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TRange1>
        constexpr auto contains(const TRange1& range) -> bool
            requires(RFwdRange<TRange1>)
                    and (REqualityComparableWith<TElem, typename TRange1::TElem>)
        {
            return _Impl::findRange(range) != _Impl::iterEnd();
        }

    //// -------------------------------------------------------------------------------------------
    //// Comparision
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TRange1>
        constexpr auto compare(const TRange1& range) -> i8
            requires(RRange<TRange1>) and (REqualityComparableWith<TElem, typename TRange1::TElem>)
        {
            return _Impl::compare(range);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TRange1>
        constexpr auto equals(const TRange1& range) -> bool
            requires(RRange<TRange1>) and (REqualityComparableWith<TElem, typename TRange1::TElem>)
        {
            return _Impl::compare(range) == 0;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TRange1>
        constexpr auto operator==(const TRange1& range) -> bool
            requires(RRange<TRange1>) and (REqualityComparableWith<TElem, typename TRange1::TElem>)
        {
            return _Impl::compare(range) == 0;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TRange1>
        constexpr auto operator!=(const TRange1& range) -> bool
            requires(RRange<TRange1>) and (REqualityComparableWith<TElem, typename TRange1::TElem>)
        {
            return _Impl::compare(range) != 0;
        }

    //// -------------------------------------------------------------------------------------------
    //// Helpers
    ////
    //// # To Do
    ////
    //// - Add range helper functions.
    //// -------------------------------------------------------------------------------------------

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto canGetCount() const -> bool
        {
            return RFwdIterPair<TIter, TIterEnd>;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto getCount() const -> usize
        {
            return _Impl::getCount();
        }
    };
}
