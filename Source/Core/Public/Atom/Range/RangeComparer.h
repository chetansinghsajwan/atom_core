#pragma once
#include "_StdIter.h"
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
            _StdIterWrapForAtom stdIter1{ range1.Iter() };
            _StdIterWrapForAtom stdIterEnd1{ range1.IterEnd() };
            _StdIterWrapForAtom stdIter2{ range2.Iter() };
            _StdIterWrapForAtom stdIterEnd2{ range2.IterEnd() };

            return std::equal(stdIter1, stdIterEnd1, stdIter2, stdIterEnd2);
        }
    };
}