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
    class Wrap: __VA_ARGS__                                      \
    {                                                             \
        pub pub cexpr ctor Wrap(TRange& range) noex:                   \
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
    class _RangeWrapBase
    {
        pub using TElem = typename TRange::TElem;
        pub using TIter = TIterWrap<typename TRange::TIter>;
        pub using TIterEnd = TIterWrap<typename TRange::TIterEnd>;

        pub cexpr ctor _RangeWrapBase(TRange& range) noex:
            _range{ range } { }

        pub cexpr fn iter() const noex -> TIter
        {
            return TIter{ _range.iter() };
        }

        pub cexpr fn iterEnd() const noex -> TIterEnd
        {
            return TIterEnd{ _range.iterEnd() };
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
    class _MutRangeWrapBase: _RangeWrapBase<TRange, TIterWrap>
    {
        pub using Base = _RangeWrapBase<TRange, TIterWrap>;
        pub using TMutIter = TMutIterWrap<typename TRange::TMutIter>;
        pub using TMutIterEnd = TMutIterWrap<typename TRange::TMutIterEnd>;

        pub cexpr ctor _MutRangeWrapBase(TRange& range) noex:
            Base{ range } { }

        pub cexpr fn mutIter() noex -> TMutIter
        {
            return TMutIter{ self._range.mutIter() };
        }

        pub cexpr fn mutIterEnd() noex -> TMutIterEnd
        {
            return TMutIterEnd{ self._range.mutIterEnd() };
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

#undef _RANGE_REF_WRAP