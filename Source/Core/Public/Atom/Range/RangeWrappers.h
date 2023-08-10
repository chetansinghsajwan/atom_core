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
    public:
        constexpr Wrap(TRange& range):                   \
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
    requires(RRange<TRange>)
    class _RangeWrapBase
    {
    public:
        using TElem = typename TRange::TElem;
        using TIter = TIterWrap<typename TRange::TIter>;
        using TIterEnd = TIterWrap<typename TRange::TIterEnd>;

    public:
        constexpr _RangeWrapBase(TRange& range):
            _range{ range } { }

    public:
        constexpr fn iter() const -> TIter
        {
            return TIter{ _range.iter() };
        }

        constexpr fn iterEnd() const -> TIterEnd
        {
            return TIterEnd{ _range.iterEnd() };
        }

    private:
        TRange& _range;
    };

    template
    <
        typename TRange,
        template<typename TIter> typename TIterWrap,
        template<typename TIter> typename TMutIterWrap
    >
    requires(RMutRange<TRange>)
    class _MutRangeWrapBase: _RangeWrapBase<TRange, TIterWrap>
    {
    public:
        using Base = _RangeWrapBase<TRange, TIterWrap>;
        using TMutIter = TMutIterWrap<typename TRange::TMutIter>;
        using TMutIterEnd = TMutIterWrap<typename TRange::TMutIterEnd>;

    public:
        constexpr _MutRangeWrapBase(TRange& range):
            Base{ range } { }

    public:
        constexpr fn mutIter() -> TMutIter
        {
            return TMutIter{ self._range.mutIter() };
        }

        constexpr fn mutIterEnd() -> TMutIterEnd
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