#pragma once
#include "RangeReq.h"

namespace Atom
{
    struct RangeHelper
    {
        template <typename TRange>
        requires RRangeOf<TRange, typename TRange::TElem>
        constexpr bool CanGetRangeCount() const noexcept
        {
            return RFwdRangeOf<TRange, typename TRange::TElem>;
        }

        template <typename TRange>
        requires RRangeOf<TRange, typename TRange::TElem>
        constexpr usize GetRangeCount(const TRange& range) const noexcept
        {
            if constexpr (RJumpRangeOf<TRange, typename TRange::TElem>)
            {
                return range.IterEnd() - range.Iter();
            }

            usize count = 0;
            for (auto& el : range) count++;
            return count;
        }
    };
}