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
        template <tname TRange1, tname TRange2>
        requires RFwdRange<TRange1>
            and RFwdRange<TRange2>
        cexpr fn Find(const TRange1& range1, const TRange2& range2) -> tname TRange1::TIter 
        {
            StdIterWrapForAtomIter stdIter1{ range1.Iter() };
            StdIterWrapForAtomIter stdIterEnd1{ range1.IterEnd() };
            StdIterWrapForAtomIter stdIter2{ range2.Iter() };
            StdIterWrapForAtomIter stdIterEnd2{ range2.IterEnd() };
            return std::find(stdIter1, stdIterEnd1, stdIter2, stdIterEnd2);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <tname TRange>
        requires RFwdRange<TRange>
        cexpr fn Find(const TRange& range, const tname TRange::TElem& el) -> tname TRange::TIter 
        {
            StdIterWrapForAtomIter stdIter{ range.Iter() };
            StdIterWrapForAtomIter stdIterEnd{ range.IterEnd() };
            return std::find(stdIter, stdIterEnd, el).iter;
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <tname TRange>
        requires RFwdRange<TRange>
        cexpr fn Contains(const TRange& range, const tname TRange::TElem& el) -> bool
        {
            return Find(range, el) != range.IterEnd();
        }

        /// ----------------------------------------------------------------------------------------
        /// @TODO: Add comparision requirements for TElem of both ranges.
        /// ----------------------------------------------------------------------------------------
        template <tname TRange1, tname TRange2>
        requires RFwdRange<TRange1>
            and RFwdRange<TRange2>
        cexpr fn Contains(const TRange1& range1, const TRange2& range2) -> bool
        {
            return Find(range1, range2) != range1.IterEnd();
        }
    };
}