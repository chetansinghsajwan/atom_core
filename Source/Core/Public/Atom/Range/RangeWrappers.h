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
        using TMutIter = TMutIterWrap<typename TRange::TMutIter>;
        using TMutIterEnd = TMutIterWrap<typename TRange::TMutIterEnd>;

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
    struct RangeRefWrap: _RangeWrapBase<TRange, IterWrap> { };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    struct MutRangeRefWrap: _MutRangeWrapBase<TRange, IterWrap, MutIterWrap> { };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    struct FwdRangeRefWrap: _RangeWrapBase<TRange, FwdIterWrap> { };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    struct MutFwdRangeRefWrap: _MutRangeWrapBase<TRange, FwdIterWrap, MutFwdIterWrap> { };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    struct BidiRangeRefWrap: _RangeWrapBase<TRange, BidiIterWrap> { };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    struct MutBidiRangeRefWrap: _MutRangeWrapBase<TRange, BidiIterWrap, MutBidiIterWrap> { };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    struct JumpRangeRefWrap: _RangeWrapBase<TRange, JumpIterWrap> { };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    struct MutJumpRangeRefWrap: _MutRangeWrapBase<TRange, JumpIterWrap, MutJumpIterWrap> { };
}