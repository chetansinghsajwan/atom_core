#pragma once
#include "IterWrappers.h"
#include "RangeReq.h"

#pragma region Impl

/// ------------------------------------------------------------------------------------------------
/// Clang P1814R0: Class template argument deduction for alias templates.
/// Clang doesn't support that feature yet, so we use this implementation as a workaround.
/// 
/// @TODO: Use the commented implementation once that feature is implemented.
/// ------------------------------------------------------------------------------------------------
#define _RANGE_REF_WRAP(Wrap, ...)                                \
    struct Wrap: __VA_ARGS__                                      \
    {                                                             \
        constexpr Wrap(TRange& range) noexcept:                   \
            __VA_ARGS__{ range } { }                              \
    }

// Preferred implementation.
// 
// #define _RANGE_REF_WRAP(Wrap, ...)\
//     using Wrap = __VA_ARGS__;

#pragma endregion

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
    _RANGE_REF_WRAP(RangeRefWrap, _RangeWrapBase<TRange, IterWrap>);

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    _RANGE_REF_WRAP(MutRangeRefWrap, _MutRangeWrapBase<TRange, IterWrap, MutIterWrap>);

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    _RANGE_REF_WRAP(FwdRangeRefWrap, _RangeWrapBase<TRange, FwdIterWrap>);

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    _RANGE_REF_WRAP(MutFwdRangeRefWrap, _MutRangeWrapBase<TRange, FwdIterWrap, MutFwdIterWrap>);

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    _RANGE_REF_WRAP(BidiRangeRefWrap, _RangeWrapBase<TRange, BidiIterWrap>);

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    _RANGE_REF_WRAP(MutBidiRangeRefWrap, _MutRangeWrapBase<TRange, BidiIterWrap, MutBidiIterWrap>);

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    _RANGE_REF_WRAP(JumpRangeRefWrap, _RangeWrapBase<TRange, JumpIterWrap>);

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    _RANGE_REF_WRAP(MutJumpRangeRefWrap, _MutRangeWrapBase<TRange, JumpIterWrap, MutJumpIterWrap>);
}