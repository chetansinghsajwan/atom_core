#pragma once
#include "IterWrappers.h"
#include "RangeReq.h"

namespace Atom
{
    template
    <
        typename TRange,
        template<typename TIter> typename TIterWrap
    >
    requires RRange<TRange>
    struct _RangeWrapBase
    {
        using TElem = typename TRange::TElem;
        using TIter = TIterWrap<typename TRange::TIter>;
        using TIterEnd = TIterWrap<typename TRange::TIterEnd>;

        constexpr _RangeWrapBase(TRange& range) noexcept:
            _range{ range } { }

        constexpr TIter Iter() const noexcept
        {
            return TIter{ _range.Iter() };
        }

        constexpr TIterEnd IterEnd() const noexcept
        {
            return TIterEnd{ _range.IterEnd() };
        }

        TRange& _range;
    };

    template
    <
        typename TRange,
        template<typename TIter> typename TIterWrap,
        template<typename TIter> typename TMutIterWrap
    >
    requires RMutRange<TRange>
    struct _MutRangeWrapBase: _RangeWrapBase<TRange, TIterWrap>
    {
        using Base = _RangeWrapBase<TRange, TIterWrap>;
        using TMutIter = TMutIterWrap<typename TRange::TMutIter>;
        using TMutIterEnd = TMutIterWrap<typename TRange::TMutIterEnd>;

        constexpr _MutRangeWrapBase(TRange& range) noexcept:
            Base{ range } { }

        constexpr TMutIter MutIter() noexcept
        {
            return TMutIter{ this->_range.MutIter() };
        }

        constexpr TMutIterEnd MutIterEnd() noexcept
        {
            return TMutIterEnd{ this->_range.MutIterEnd() };
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    using RangeRefWrap = _RangeWrapBase<TRange, IterWrap>;

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    using MutRangeRefWrap = _MutRangeWrapBase<TRange, IterWrap, MutIterWrap>;

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    using FwdRangeRefWrap = _RangeWrapBase<TRange, FwdIterWrap>;

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    using MutFwdRangeRefWrap = _MutRangeWrapBase<TRange, FwdIterWrap, MutFwdIterWrap>;

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    using BidiRangeRefWrap = _RangeWrapBase<TRange, BidiIterWrap>;

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    using MutBidiRangeRefWrap = _MutRangeWrapBase<TRange, BidiIterWrap, MutBidiIterWrap>;

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    using JumpRangeRefWrap = _RangeWrapBase<TRange, JumpIterWrap>;

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    using MutJumpRangeRefWrap = _MutRangeWrapBase<TRange, JumpIterWrap, MutJumpIterWrap>;
}