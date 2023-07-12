#pragma once
#include "RangeReq.h"
#include "RangeComparer.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname TRange>
    requires RRange<TRange>
    constexpr auto begin(const TRange& range) noex
    {
        return range.Iter();
    }

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname TRange>
    requires RMutRange<TRange>
    constexpr auto begin(TRange& range) noex
    {
        return range.MutIter();
    }

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname TRange>
    requires RRange<TRange>
    constexpr auto end(const TRange& range) noex
    {
        return range.IterEnd();
    }

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname TRange>
    requires RMutRange<TRange>
    constexpr auto end(TRange& range) noex
    {
        return range.MutIterEnd();
    }

    /// --------------------------------------------------------------------------------------------
    /// @TODO: Add comparision requirements for TElem of both ranges.
    /// --------------------------------------------------------------------------------------------
    template <tname TRange1, tname TRange2>
    requires RRange<TRange1> && RRange<TRange2>
    constexpr bool operator ==(const TRange1& range1, const TRange2& range2) noex
    {
        return RangeComparer().AreEqual(range1, range2);
    }

    /// --------------------------------------------------------------------------------------------
    /// @TODO: Add comparision requirements for TElem of both ranges.
    /// --------------------------------------------------------------------------------------------
    template <tname TRange1, tname TRange2>
    requires RRange<TRange1> && RRange<TRange2>
    constexpr bool operator !=(const TRange1& range1, const TRange2& range2) noex
    {
        return !RangeComparer().AreEqual(range1, range2);
    }
}