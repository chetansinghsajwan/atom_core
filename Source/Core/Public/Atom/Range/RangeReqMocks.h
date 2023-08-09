#pragma once
#include "RangeReqChecks.h"
#include "IterReqMocks.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname _TIter, tname _TIterEnd, tname T>
    class _RangeReqMock
    {
        pub using TElem = T;
        pub using TIter = _TIter;
        pub using TIterEnd = _TIterEnd;

        pub fn iter() const -> TIter;
        pub fn iterEnd() const -> TIterEnd;
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname _TIter, tname _TIterEnd, tname T>
    class _MutRangeReqMock
    {
        pub using TElem = T;
        pub using TIter = _TIter;
        pub using TIterEnd = _TIterEnd;
        pub using TMutIter = _TIter;
        pub using TMutIterEnd = _TIterEnd;

        pub fn iter() const -> TIter;
        pub fn iterEnd() const -> TIterEnd;
        pub fn mutIter() -> TMutIter;
        pub fn mutIterEnd() -> TMutIterEnd;
    };

    /// --------------------------------------------------------------------------------------------
    /// {RRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    class RangeReqMock extends
        _RangeReqMock<IterReqMock<T>, IterEndReqMock, T> { };

    ATOM_SATISFIES_RANGE_TEMP(RangeReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RMutRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    class MutRangeReqMock extends
        _MutRangeReqMock<MutIterReqMock<T>, IterEndReqMock, T> { };

    ATOM_SATISFIES_MUT_RANGE_TEMP(MutRangeReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RFwdRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    class FwdRangeReqMock extends
        _RangeReqMock<FwdIterReqMock<T>, IterEndReqMock, T> { };

    ATOM_SATISFIES_FWD_RANGE_TEMP(FwdRangeReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RMutFwdRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    class MutFwdRangeReqMock extends
        _MutRangeReqMock<MutFwdIterReqMock<T>, IterEndReqMock, T> { };

    ATOM_SATISFIES_MUT_FWD_RANGE_TEMP(MutFwdRangeReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RBidiRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    class BidiRangeReqMock extends
        _RangeReqMock<BidiIterReqMock<T>, IterEndReqMock, T> { };

    ATOM_SATISFIES_BIDI_RANGE_TEMP(BidiRangeReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RMutBidiRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    class MutBidiRangeReqMock extends
        _MutRangeReqMock<MutBidiIterReqMock<T>, IterEndReqMock, T> { };

    ATOM_SATISFIES_MUT_BIDI_RANGE_TEMP(MutBidiRangeReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RJumpRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    class JumpRangeReqMock extends
        _RangeReqMock<JumpIterReqMock<T>, IterEndReqMock, T> { };

    ATOM_SATISFIES_JUMP_RANGE_TEMP(JumpRangeReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RMutJumpRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    class MutJumpRangeReqMock extends
        _MutRangeReqMock<MutJumpIterReqMock<T>, IterEndReqMock, T> { };

    ATOM_SATISFIES_MUT_JUMP_RANGE_TEMP(MutJumpRangeReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RArrRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    class ArrRangeReqMock extends
        _RangeReqMock<ArrIterReqMock<T>, ArrIterReqMock<T>, T> { };

    ATOM_SATISFIES_ARR_RANGE_TEMP(ArrRangeReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RMutArrRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    class MutArrRangeReqMock extends
        _MutRangeReqMock<MutArrIterReqMock<T>, MutArrIterReqMock<T>, T> { };

    ATOM_SATISFIES_MUT_ARR_RANGE_TEMP(MutArrRangeReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RCommonRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    class CommonRangeReqMock extends 
        _RangeReqMock<IterReqMock<T>, IterReqMock<T>, T> { };

    ATOM_SATISFIES_COMMON_RANGE_TEMP(CommonRangeReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RMutCommonRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    class MutCommonRangeReqMock extends
        _MutRangeReqMock<MutFwdIterReqMock<T>, MutFwdIterReqMock<T>, T> { };

    ATOM_SATISFIES_MUT_COMMON_RANGE_TEMP(MutCommonRangeReqMock);
}
