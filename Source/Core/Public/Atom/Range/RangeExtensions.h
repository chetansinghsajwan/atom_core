#pragma once
#include "RangeReq.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// @TODO: Refactor this.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    requires RRangeOf<TTI::TRemoveCVRef<TRange>, typename TTI::TRemoveCVRef<TRange>::TElem>
    constexpr auto begin(const TRange& range) noexcept
    {
        // if constexpr (!RConst<TRange> && RMutFwdRangeOf<TTI::TRemoveCVRef<TRange>, 
        //     typename TTI::TRemoveCVRef<TRange>::TElem>)
        // {
        //     return range.MutIter();
        // }
        // else
        {
            return range.Iter();
        }
    }

    template <typename TRange>
    requires RMutFwdRangeOf<TRange, typename TRange::TElem>
    constexpr auto begin(TRange& range) noexcept
    {
        return range.MutIter();
    }

    /// --------------------------------------------------------------------------------------------
    /// @TODO: Refactor this.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    requires RRangeOf<TTI::TRemoveCVRef<TRange>, typename TTI::TRemoveCVRef<TRange>::TElem>
    constexpr auto end(const TRange& range) noexcept
    {
        // if constexpr (!RConst<TRange> && RMutFwdRangeOf<TTI::TRemoveCVRef<TRange>, 
        //     typename TTI::TRemoveCVRef<TRange>::TElem>)
        // {
        //     return range.MutIterEnd();
        // }
        // else
        {
            return range.IterEnd();
        }
    }

    template <typename TRange>
    requires RMutFwdRangeOf<TRange, typename TRange::TElem>
    constexpr auto end(TRange&& range) noexcept
    {
        return range.MutIterEnd();
    }

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRange1, typename TRange2>
    requires 
        RRangeOf<TRange1, typename TRange1::TElem> &&
        RRangeOf<TRange2, typename TRange2::TElem>
    constexpr bool operator ==(const TRange1& range1, const TRange2& range2) noexcept
    {
        return _Compare(range1, range2) == 0;
    }

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRange1, typename TRange2>
    requires 
        RRangeOf<TRange1, typename TRange1::TElem> &&
        RRangeOf<TRange2, typename TRange2::TElem>
    constexpr bool operator !=(const TRange1& range1, const TRange2& range2) noexcept
    {
        return !(range1 == range2);
    }

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRange1, typename TRange2>
    constexpr int8 _Compare(const TRange1& range1, const TRange2& range2) noexcept
    {
        // TODO: Implement this.
        return 0;
    }
}