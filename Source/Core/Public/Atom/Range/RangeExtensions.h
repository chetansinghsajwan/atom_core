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
    constexpr fn begin(const TRange& range) noex -> auto
    {
        return range.Iter();
    }

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname TRange>
    requires RMutRange<TRange>
    constexpr fn begin(TRange& range) noex -> auto
    {
        return range.MutIter();
    }

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname TRange>
    requires RRange<TRange>
    constexpr fn end(const TRange& range) noex -> auto
    {
        return range.IterEnd();
    }

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname TRange>
    requires RMutRange<TRange>
    constexpr fn end(TRange& range) noex -> auto
    {
        return range.MutIterEnd();
    }

    /// --------------------------------------------------------------------------------------------
    /// @TODO: Add comparision requirements for TElem of both ranges.
    /// --------------------------------------------------------------------------------------------
    template <tname TRange1, tname TRange2>
    requires RRange<TRange1> && RRange<TRange2>
    constexpr fn operator ==(const TRange1& range1, const TRange2& range2) noex -> bool
    {
        return RangeComparer().AreEqual(range1, range2);
    }

    /// --------------------------------------------------------------------------------------------
    /// @TODO: Add comparision requirements for TElem of both ranges.
    /// --------------------------------------------------------------------------------------------
    template <tname TRange1, tname TRange2>
    requires RRange<TRange1> && RRange<TRange2>
    constexpr fn operator !=(const TRange1& range1, const TRange2& range2) noex -> bool
    {
        return !RangeComparer().AreEqual(range1, range2);
    }
}