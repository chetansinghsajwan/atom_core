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
    constexpr fn begin(const TRange& range) -> auto
    {
        return range.iter();
    }

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    requires RMutRange<TRange>
    constexpr fn begin(TRange& range) -> auto
    {
        return range.mutIter();
    }

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    requires RRange<TRange>
    constexpr fn end(const TRange& range) -> auto
    {
        return range.iterEnd();
    }

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    requires RMutRange<TRange>
    constexpr fn end(TRange& range) -> auto
    {
        return range.mutIterEnd();
    }

    /// --------------------------------------------------------------------------------------------
    /// @TODO: Add comparision requirements for TElem of both ranges.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange1, typename TRange2>
    requires RRange<TRange1> && RRange<TRange2>
    constexpr fn op==(const TRange1& range1, const TRange2& range2) -> bool
    {
        return RangeComparer().AreEqual(range1, range2);
    }

    /// --------------------------------------------------------------------------------------------
    /// @TODO: Add comparision requirements for TElem of both ranges.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange1, typename TRange2>
    requires RRange<TRange1> && RRange<TRange2>
    constexpr fn op!=(const TRange1& range1, const TRange2& range2) -> bool
    {
        return !RangeComparer().AreEqual(range1, range2);
    }
}