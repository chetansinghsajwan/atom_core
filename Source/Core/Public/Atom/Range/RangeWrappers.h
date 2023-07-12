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
        cexpr ctor Wrap(TRange& range) noex:                   \
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
        tname TRange,
        template<tname TIter> tname TIterWrap
    >
    requires RRange<TRange>
    struct _RangeWrapBase
    {
        using TElem = tname TRange::TElem;
        using TIter = TIterWrap<tname TRange::TIter>;
        using TIterEnd = TIterWrap<tname TRange::TIterEnd>;

        cexpr ctor _RangeWrapBase(TRange& range) noex:
            _range{ range } { }

        cexpr fn Iter() const noex -> TIter
        {
            return TIter{ _range.Iter() };
        }

        cexpr fn IterEnd() const noex -> TIterEnd
        {
            return TIterEnd{ _range.IterEnd() };
        }

        TRange& _range;
    };

    template
    <
        tname TRange,
        template<tname TIter> tname TIterWrap,
        template<tname TIter> tname TMutIterWrap
    >
    requires RMutRange<TRange>
    struct _MutRangeWrapBase: _RangeWrapBase<TRange, TIterWrap>
    {
        using Base = _RangeWrapBase<TRange, TIterWrap>;
        using TMutIter = TMutIterWrap<tname TRange::TMutIter>;
        using TMutIterEnd = TMutIterWrap<tname TRange::TMutIterEnd>;

        cexpr ctor _MutRangeWrapBase(TRange& range) noex:
            Base{ range } { }

        cexpr fn MutIter() noex -> TMutIter
        {
            return TMutIter{ this->_range.MutIter() };
        }

        cexpr fn MutIterEnd() noex -> TMutIterEnd
        {
            return TMutIterEnd{ this->_range.MutIterEnd() };
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname TRange>
    _RANGE_REF_WRAP(RangeRefWrap, _RangeWrapBase<TRange, IterWrap>);

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname TRange>
    _RANGE_REF_WRAP(MutRangeRefWrap, _MutRangeWrapBase<TRange, IterWrap, MutIterWrap>);

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname TRange>
    _RANGE_REF_WRAP(FwdRangeRefWrap, _RangeWrapBase<TRange, FwdIterWrap>);

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname TRange>
    _RANGE_REF_WRAP(MutFwdRangeRefWrap, _MutRangeWrapBase<TRange, FwdIterWrap, MutFwdIterWrap>);

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname TRange>
    _RANGE_REF_WRAP(BidiRangeRefWrap, _RangeWrapBase<TRange, BidiIterWrap>);

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname TRange>
    _RANGE_REF_WRAP(MutBidiRangeRefWrap, _MutRangeWrapBase<TRange, BidiIterWrap, MutBidiIterWrap>);

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname TRange>
    _RANGE_REF_WRAP(JumpRangeRefWrap, _RangeWrapBase<TRange, JumpIterWrap>);

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname TRange>
    _RANGE_REF_WRAP(MutJumpRangeRefWrap, _MutRangeWrapBase<TRange, JumpIterWrap, MutJumpIterWrap>);
}

#undef _RANGE_REF_WRAP