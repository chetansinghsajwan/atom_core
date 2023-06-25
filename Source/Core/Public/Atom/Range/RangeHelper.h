#pragma once
#include "RangeReq.h"

namespace Atom
{
    struct RangeHelper
    {
        template <typename TRange>
        requires RRange<TRange>
        constexpr bool CanGetRangeCount() const noexcept
        {
            return RFwdRange<TRange>;
        }

        template <typename TRange>
        requires RRange<TRange>
        constexpr usize GetRangeCount(const TRange& range) const noexcept
        {
            if constexpr (RJumpRange<TRange>)
            {
                return range.IterEnd() - range.Iter();
            }

            usize count = 0;
            for (auto& el : range) count++;
            return count;
        }
    };
}