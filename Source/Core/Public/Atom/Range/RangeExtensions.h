#pragma once
#include "StdIterWrapForAtomIter.h"
#include "RangeComparer.h"
#include "RangeReq.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
        requires RRange<TRange>
    constexpr auto begin(const TRange& range) -> auto
    {
        return StdIterWrapForAtomIter{ range.iter() };
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
        requires RMutRange<TRange>
    constexpr auto begin(TRange& range) -> auto
    {
        return StdIterWrapForAtomIter{ range.mutIter() };
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
        requires RRange<TRange>
    constexpr auto end(const TRange& range) -> auto
    {
        return StdIterWrapForAtomIter{ range.iterEnd() };
    }

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
        requires RMutRange<TRange>
    constexpr auto end(TRange& range) -> auto
    {
        return StdIterWrapForAtomIter{ range.mutIterEnd() };
    }

    /// --------------------------------------------------------------------------------------------
    /// @TODO: Add comparision requirements for TElem of both ranges.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange1, typename TRange2>
        requires RRange<TRange1> && RRange<TRange2>
    constexpr auto operator==(const TRange1& range1, const TRange2& range2) -> bool
    {
        return RangeComparer().AreEqual(range1, range2);
    }

    /// --------------------------------------------------------------------------------------------
    /// @TODO: Add comparision requirements for TElem of both ranges.
    /// --------------------------------------------------------------------------------------------
    template <typename TRange1, typename TRange2>
        requires RRange<TRange1> && RRange<TRange2>
    constexpr auto operator!=(const TRange1& range1, const TRange2& range2) -> bool
    {
        return !RangeComparer().AreEqual(range1, range2);
    }
}
