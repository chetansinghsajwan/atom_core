#pragma once
#include "ArrayIter.h"
#include "RangeReq.h"
#include "StdIterWrapForAtomIter.h"

#include <algorithm>

namespace Atom
{
    template <typename TRange>
    class RangeTraitImpl;

    template <typename TUserImpl>
    class _RangeTraitImpl
    {
    public:
        using TImpl = TUserImpl;
        using TElem = typename TImpl::TElem;
        using TIter = typename TImpl::TIter;
        using TIterEnd = typename TImpl::TIterEnd;

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto iter() const -> TIter
        {
            return _impl.iter();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto iterEnd() const -> TIterEnd
        {
            return _impl.iterEnd();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TElem1>
        constexpr auto findElem(const TElem1& el) const -> TIter
        {
            StdIterWrapForAtomIter stdIter{ iter() };
            StdIterWrapForAtomIter stdIterEnd{ iterEnd() };
            return std::find(stdIter, stdIterEnd, el).iter;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TRange1>
        constexpr auto findRange(const TRange1& range) const -> TIter
        {
            StdIterWrapForAtomIter stdIter1{ iter() };
            StdIterWrapForAtomIter stdIterEnd1{ iterEnd() };
            StdIterWrapForAtomIter stdIter2{ range.iter() };
            StdIterWrapForAtomIter stdIterEnd2{ range.iterEnd() };
            return std::find(stdIter1, stdIterEnd1, stdIter2, stdIterEnd2);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TRange1>
        auto compare(const TRange1& range) const -> i8
        {
            StdIterWrapForAtomIter stdIter1{ iter() };
            StdIterWrapForAtomIter stdIterEnd1{ iterEnd() };
            StdIterWrapForAtomIter stdIter2{ range.iter() };
            StdIterWrapForAtomIter stdIterEnd2{ range.iterEnd() };

            return std::equal(stdIter1, stdIterEnd1, stdIter2, stdIterEnd2);
        }

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

    public:
        TUserImpl _impl;
    };

    template <typename TRangeImpl>
    class RangeTraitRecursive
    {
        using Base = TRangeImpl;

    public:
        using TImpl = Base;
        using TElem = typename TImpl::TElem;
        using TIter = typename TImpl::TIter;
        using TIterEnd = typename TImpl::TIterEnd;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////
    //// Iteration
    ////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto iter() const -> TIter
        {
            return _impl.iter();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto iterEnd() const -> TIterEnd
        {
            return _impl.iterEnd();
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto begin() const
        {
            return StdIterWrapForAtomIter{ _impl.iter() };
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        constexpr auto end() const
        {
            return StdIterWrapForAtomIter{ _impl.iterEnd() };
        }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////
    //// Search
    ////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TElem1>
        constexpr auto find(const TElem1& el) const -> TIter
            requires(REqualityComparableWith<TElem, TElem1>)
        {
            return _impl.findElem(el);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TRange1>
        constexpr auto findRange(const TRange1& range) const -> TIter
            requires(RFwdRange<TRange1>)
                    and (REqualityComparableWith<TElem, typename TRange1::TElem>)
        {
            return _impl.findRange(range);
        }

        template <typename TRange1>
        auto countAny(const TRange1& range) const -> usize
        {
            usize count = 0;
            for (auto it = iter(); it.compare(iterEnd()) != 0; it++)
            for (const auto& el : range)
            {
                if (*it == el)
                    count++;
            }

            return count;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TElem1>
        constexpr auto contains(const TElem1& el) const -> bool
            requires(REqualityComparableWith<TElem, TElem1>)
        {
            return _impl.findElem(el).compare(_impl.iterEnd()) != 0;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TRange1>
        constexpr auto contains(const TRange1& range) const -> bool
            requires(RFwdRange<TRange1>)
                and (REqualityComparableWith<TElem, typename TRange1::TElem>)
        {
            return _impl.findRange(range).compare(_impl.iterEnd()) != 0;
        }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////
    //// Comparision
    ////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TRange1>
        constexpr auto compare(const TRange1& range) const -> i8
            requires(RRange<TRange1>) and (REqualityComparableWith<TElem, typename TRange1::TElem>)
        {
            return _impl.compare(range);
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TRange1>
        constexpr auto equals(const TRange1& range) const -> bool
            requires(RRange<TRange1>) and (REqualityComparableWith<TElem, typename TRange1::TElem>)
        {
            return _impl.compare(range) == 0;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TRange1>
        constexpr auto operator==(const TRange1& range) const -> bool
            requires(RRange<TRange1>) and (REqualityComparableWith<TElem, typename TRange1::TElem>)
        {
            return _impl.compare(range) == 0;
        }

        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TRange1>
        constexpr auto operator!=(const TRange1& range) const -> bool
            requires(RRange<TRange1>) and (REqualityComparableWith<TElem, typename TRange1::TElem>)
        {
            return _impl.compare(range) != 0;
        }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////
    //// Helpers
    ////
    //// # To Do
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
            return _impl.getCount();
        }

    public:
        TImpl _impl;
    };

    template <typename TRange>
    class RangeTrait: public RangeTraitRecursive<_RangeTraitImpl<TRange>> {};
}
