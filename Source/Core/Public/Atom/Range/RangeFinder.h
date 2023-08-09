#pragma once
#include "RangeReq.h"
#include "StdIterWrapForAtomIter.h"

#include <algorithm>

namespace Atom
{
    class RangeFinder
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// @TODO: Add comparision requirements for TElem of both ranges.
        /// ----------------------------------------------------------------------------------------
        template <typename TRange1, typename TRange2>
        requires RFwdRange<TRange1>
            and RFwdRange<TRange2>
        cexpr fn Find(const TRange1& range1, const TRange2& range2) -> typename TRange1::TIter 
        {
            StdIterWrapForAtomIter stdIter1{ range1.iter() };
            StdIterWrapForAtomIter stdIterEnd1{ range1.iterEnd() };
            StdIterWrapForAtomIter stdIter2{ range2.iter() };
            StdIterWrapForAtomIter stdIterEnd2{ range2.iterEnd() };
            return std::find(stdIter1, stdIterEnd1, stdIter2, stdIterEnd2);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        requires RFwdRange<TRange>
        cexpr fn Find(const TRange& range, const typename TRange::TElem& el) -> typename TRange::TIter 
        {
            StdIterWrapForAtomIter stdIter{ range.iter() };
            StdIterWrapForAtomIter stdIterEnd{ range.iterEnd() };
            return std::find(stdIter, stdIterEnd, el).iter;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        requires RFwdRange<TRange>
        cexpr fn Contains(const TRange& range, const typename TRange::TElem& el) -> bool
        {
            return Find(range, el) != range.iterEnd();
        }

        /// ----------------------------------------------------------------------------------------
        /// @TODO: Add comparision requirements for TElem of both ranges.
        /// ----------------------------------------------------------------------------------------
        template <typename TRange1, typename TRange2>
        requires RFwdRange<TRange1>
            and RFwdRange<TRange2>
        cexpr fn Contains(const TRange1& range1, const TRange2& range2) -> bool
        {
            return Find(range1, range2) != range1.iterEnd();
        }
    };
}