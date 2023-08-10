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
        pub pub constexpr ctor Wrap(TRange& range):                   \
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

        pub constexpr ctor _RangeWrapBase(TRange& range):
            _range{ range } { }

        pub constexpr fn iter() const -> TIter
        {
            return TIter{ _range.iter() };
        }

        pub constexpr fn iterEnd() const -> TIterEnd
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

        pub constexpr ctor _MutRangeWrapBase(TRange& range):
            Base{ range } { }

        pub constexpr fn mutIter() -> TMutIter
        {
            return TMutIter{ self._range.mutIter() };
        }

        pub constexpr fn mutIterEnd() -> TMutIterEnd
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