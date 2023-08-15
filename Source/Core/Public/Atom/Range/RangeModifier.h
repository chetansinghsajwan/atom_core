#pragma once
#include "Atom/Invokable/Invokable.h"
#include "RangeReq.h"

namespace Atom
{
    class RangeModifier
    {
    public:
        /// ----------------------------------------------------------------------------------------
        ///
        /// ----------------------------------------------------------------------------------------
        template <typename TRange, typename TPred>
            requires RRange<TRange> and RInvokable<TPred, bool(const typename TRange::TElem&)>
        constexpr auto RemoveIf(TRange& range, TPred&& pred) -> usize
        {
            usize count = 0;
            auto itEnd = range.iterEnd();
            for (auto it = range.iter(); it.equals(itEnd); it.next())
            {
                if (pred(it.value()))
                {
                    it = range.RemoveAt(it);
                    count++;
                }
            }

            return count;
        }
    };
}
