#pragma once
#include "RangeReq.h"
#include "StdIterWrapForAtomIter.h"

#include <algorithm>

namespace Atom
{
    template <typename T>
    class RangeTraitImpl;

    template <typename T>
    class _RangeTraitImpl:
        private RangeTraitImpl<T>
    {
        using _Impl = RangeTraitImpl<T>;

    public:
        using TElem = typename _Impl::TElem;
        using TIter = typename _Impl::TIter;
        using TIterEnd = typename _Impl::TIterEnd;

    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr fn iter() const -> TIter
        {
            return _Impl::iter();
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr fn iterEnd() const -> TIterEnd
        {
            return _Impl::iterEnd();
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename TElem1>
        constexpr fn findElem(const TElem1& el) -> TIter
        {
            StdIterWrapForAtomIter stdIter{ iter() };
            StdIterWrapForAtomIter stdIterEnd{ iterEnd() };
            return std::find(stdIter, stdIterEnd, el).iter;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        constexpr fn findRange(const TRange& range) -> TIter
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
        template <typename TRange>
        fn compare(const TRange& range) -> i8
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
        constexpr fn getCount() const -> usize
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
        using TraitImpl = RangeTraitImpl<TRange>;
        using _Impl = _RangeTraitImpl<TRange>;

    public:
        using TElem = typename TraitImpl::TElem;
        using TIter = typename TraitImpl::TIter;
        using TIterEnd = typename TraitImpl::TIterEnd;

    //// -------------------------------------------------------------------------------------------
    //// Iteration
    //// -------------------------------------------------------------------------------------------

        /// --------------------------------------------------------------------------------------------
        /// 
        /// --------------------------------------------------------------------------------------------
        constexpr fn iter() const -> TIter
        {
            return _Impl::iter();
        }

        /// --------------------------------------------------------------------------------------------
        /// 
        /// --------------------------------------------------------------------------------------------
        constexpr fn iterEnd() const -> TIterEnd
        {
            return _Impl::iterEnd();
        }

        /// --------------------------------------------------------------------------------------------
        /// 
        /// --------------------------------------------------------------------------------------------
        constexpr fn begin() const -> TIter
        {
            return _Impl::iter();
        }

        /// --------------------------------------------------------------------------------------------
        /// 
        /// --------------------------------------------------------------------------------------------
        constexpr fn end() const -> TIterEnd
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
        constexpr fn find(const TElem1& el) -> TIter 
            requires(REqualityComparableWith<TElem, TElem1>)
        {
            return _Impl::findElem(el);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename TRange1>
        constexpr fn findRange(const TRange1& range) -> TIter
            requires(RFwdRange<TRange1>)
                and (REqualityComparableWith<TElem, typename TRange1::TElem>)
        {
            return _Impl::findRange(range);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename TElem1>
        constexpr fn contains(const TElem1& el) -> bool
            requires(REqualityComparableWith<TElem, TElem1>)
        {
            return _Impl::findElem(el) != _Impl::iterEnd();
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename TRange1>
        constexpr fn contains(const TRange1& range) -> bool
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
        constexpr fn compare(const TRange1& range) -> i8
            requires(RRange<TRange1>)
                and (REqualityComparableWith<TElem, typename TRange1::TElem>)
        {
            return _Impl::compare(range);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename TRange1>
        constexpr fn equals(const TRange1& range) -> bool
            requires(RRange<TRange1>)
                and (REqualityComparableWith<TElem, typename TRange1::TElem>)
        {
            return _Impl::compare(range) == 0;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename TRange1>
        constexpr fn op==(const TRange1& range) -> bool
            requires(RRange<TRange1>)
                and (REqualityComparableWith<TElem, typename TRange1::TElem>)
        {
            return _Impl::compare(range) == 0;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename TRange1>
        constexpr fn op!=(const TRange1& range) -> bool
            requires(RRange<TRange1>)
                and (REqualityComparableWith<TElem, typename TRange1::TElem>)
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
        constexpr fn canGetCount() const -> bool
        {
            return RFwdIterPair<TIter, TIterEnd>;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        constexpr fn getCount() const -> usize
        {
            return _Impl::getCount();
        }
    };
}