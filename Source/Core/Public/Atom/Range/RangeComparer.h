#pragma once
#include "RangeReq.h"

#include <algorithm>

namespace Atom
{
    class RangeComparer
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// @TODO: Update comparision requirements for TElem of both ranges.
        /// ----------------------------------------------------------------------------------------
        template <typename TRange1, typename TRange2>
        requires RFwdRange<TRange1> && RFwdRange<TRange2> &&
            RSameAs<typename TRange1::TElem, typename TRange2::TElem>
        int8 AreEqual(const TRange1& range1, const TRange2& range2)
        {
            return std::equal(range1.Iter(), range1.IterEnd(),
                range2.Iter(), range2.IterEnd());
        }
    };
}