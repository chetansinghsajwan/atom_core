#pragma once
#include "IterWrappers.h"
#include "Range.h"
#include "RangeReq.h"
#include "RangeReqMocks.h"

namespace Atom
{
    template <typename TRange, template<typename TIter> typename TIterWrap>
    using _RangeWrapBase = Range
    <
        TIterWrap<typename TRange::TIter>,
        TIterWrap<typename TRange::TIterEnd>
    >;

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    struct RangeWrap: _RangeWrapBase<TRange, IterWrap> { };

    template <typename T>
    using _RangeWrapTestMock = RangeWrap<MutArrRangeReqMock<T>>;

    ATOM_SATISFIES_RANGE_TEMP(_RangeWrapTestMock);

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    struct MutRangeWrap: _RangeWrapBase<TRange, MutIterWrap> { };

    template <typename T>
    using _MutRangeWrapTestMock = MutRangeWrap<MutArrRangeReqMock<T>>;

    ATOM_SATISFIES_MUT_RANGE_TEMP(_MutRangeWrapTestMock);

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    struct FwdRangeWrap: _RangeWrapBase<TRange, FwdIterWrap> { };

    template <typename T>
    using _FwdRangeWrapTestMock = FwdRangeWrap<MutArrRangeReqMock<T>>;

    ATOM_SATISFIES_FWD_RANGE_TEMP(_FwdRangeWrapTestMock);

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    struct MutFwdRangeWrap: _RangeWrapBase<TRange, MutFwdIterWrap> { };

    template <typename T>
    using _MutFwdRangeWrapTestMock = MutFwdRangeWrap<MutArrRangeReqMock<T>>;

    ATOM_SATISFIES_MUT_FWD_RANGE_TEMP(_MutFwdRangeWrapTestMock);

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    struct BidiRangeWrap: _RangeWrapBase<TRange, BidiIterWrap> { };

    template <typename T>
    using _BidiRangeWrapTestMock = BidiRangeWrap<MutArrRangeReqMock<T>>;

    ATOM_SATISFIES_BIDI_RANGE_TEMP(_BidiRangeWrapTestMock);

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    struct MutBidiRangeWrap: _RangeWrapBase<TRange, MutBidiIterWrap> { };

    template <typename T>
    using _MutBidiRangeWrapTestMock = MutBidiRangeWrap<MutArrRangeReqMock<T>>;

    ATOM_SATISFIES_MUT_BIDI_RANGE_TEMP(_MutBidiRangeWrapTestMock);

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    struct JumpRangeWrap: _RangeWrapBase<TRange, JumpIterWrap> { };

    template <typename T>
    using _JumpRangeWrapTestMock = JumpRangeWrap<MutArrRangeReqMock<T>>;

    ATOM_SATISFIES_JUMP_RANGE_TEMP(_JumpRangeWrapTestMock);

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TRange>
    struct MutJumpRangeWrap: _RangeWrapBase<TRange, MutJumpIterWrap> { };

    template <typename T>
    using _MutJumpRangeWrapTestMock = MutJumpRangeWrap<MutArrRangeReqMock<T>>;

    ATOM_SATISFIES_MUT_JUMP_RANGE_TEMP(_MutJumpRangeWrapTestMock);
}