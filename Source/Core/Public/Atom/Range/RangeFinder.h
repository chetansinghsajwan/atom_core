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
        template <typename TRange1, typename TRange2>
        requires RFwdRange<TRange1> && RFwdRange<TRange2>
        class TRange1::TIter Find(const TRange1& range1, const TRange2& range2)
        {
            return std::find(range1.Iter(), range1.IterEnd(), range2.Iter(), range2.IterEnd());
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        requires RFwdRange<TRange>
        class TRange::TIter Find(const TRange& range, const TRange::TElem& el)
        {
            return std::find(range.Iter(), range.IterEnd(), el);
        }

        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        requires RFwdRange<TRange>
        bool Contains(const TRange& range, const TRange::TElem& el)
        {
            return Find(range, el) != range.IterEnd();
        }

        /// ----------------------------------------------------------------------------------------
        /// @TODO: Add comparision requirements for TElem of both ranges.
        /// ----------------------------------------------------------------------------------------
        template <typename TRange1, typename TRange2>
        requires RFwdRange<TRange1> && RFwdRange<TRange2>
        bool Contains(const TRange1& range1, const TRange2& range2)
        {
            return Find(range1, range2) != range1.IterEnd();
        }
    };
}