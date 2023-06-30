#pragma once
#include "RangeReq.h"
#include "Atom/Invokable/Invokable.h"

#include <algorithm>

namespace Atom
{
    class RangeModifier
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// 
        /// ----------------------------------------------------------------------------------------
        template <typename TRange, typename TPred>
        requires RRange<TRange> && RInvokable<TPred, bool(const typename TRange::TElem&)>
        usize RemoveIf(TRange& range, TPred&& pred)
        {
            usize count = 0;
            for (auto it = range.Iter(); it != range.IterEnd(); it++)
            {
                if (pred(*it))
                {
                    it = range.RemoveAt(it);
                    count++;
                }
            }

            return count;
        }
    };
}