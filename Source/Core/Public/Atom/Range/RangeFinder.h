#pragma once
#include "RangeReq.h"

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
            return std::find(range1.Iter(), range1.IterEnd(), range2.Iter(), range2.IterEnd());
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <tname TRange>
        requires RFwdRange<TRange>
        cexpr fn Find(const TRange& range, const tname TRange::TElem& el) -> tname TRange::TIter 
        {
            return std::find(range.Iter(), range.IterEnd(), el);
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