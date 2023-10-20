#pragma once
#include "StdIterWrapForAtomIter.h"
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
}
