#pragma once
#include "StdIterWrapForAtomIter.h"
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
        template <tname TRange1, tname TRange2>
        requires RFwdRange<TRange1> && RFwdRange<TRange2> &&
            RSameAs<tname TRange1::TElem, tname TRange2::TElem>
        fn AreEqual(const TRange1& range1, const TRange2& range2) -> i8
        {
            StdIterWrapForAtomIter stdIter1{ range1.iter() };
            StdIterWrapForAtomIter stdIterEnd1{ range1.iterEnd() };
            StdIterWrapForAtomIter stdIter2{ range2.iter() };
            StdIterWrapForAtomIter stdIterEnd2{ range2.iterEnd() };

            return std::equal(stdIter1, stdIterEnd1, stdIter2, stdIterEnd2);
        }
    };
}