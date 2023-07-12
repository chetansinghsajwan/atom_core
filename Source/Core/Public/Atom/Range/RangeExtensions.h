#pragma once
#include "RangeReq.h"
#include "RangeComparer.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    requires RRange<TRange>
    constexpr auto begin(const TRange& range) noex
    {
        return range.Iter();
    }

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    requires RMutRange<TRange>
    constexpr auto begin(TRange& range) noex
    {
        return range.MutIter();
    }

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    requires RRange<TRange>
    constexpr auto end(const TRange& range) noex
    {
        return range.IterEnd();
    }

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    requires RMutRange<TRange>
    constexpr auto end(TRange& range) noex
    {
        return range.MutIterEnd();
    }

    /// --------------------------------------------------------------------------------------------
    /// @TODO: Add comparision requirements for TElem of both ranges.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange1, typename TRange2>
    requires RRange<TRange1> && RRange<TRange2>
    constexpr bool operator ==(const TRange1& range1, const TRange2& range2) noex
    {
        return RangeComparer().AreEqual(range1, range2);
    }

    /// --------------------------------------------------------------------------------------------
    /// @TODO: Add comparision requirements for TElem of both ranges.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange1, typename TRange2>
    requires RRange<TRange1> && RRange<TRange2>
    constexpr bool operator !=(const TRange1& range1, const TRange2& range2) noex
    {
        return !RangeComparer().AreEqual(range1, range2);
    }
}