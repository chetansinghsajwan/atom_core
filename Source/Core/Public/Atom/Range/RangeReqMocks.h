#pragma once
#include "RangeReqChecks.h"
#include "IterReqMocks.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename _TIter, typename _TIterEnd, typename T>
    struct _RangeReqMock
    {
        using TElem = T;
        using TIter = _TIter;
        using TIterEnd = _TIterEnd;

        TIter Iter() const;
        TIterEnd IterEnd() const;
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename _TIter, typename _TIterEnd, typename T>
    struct _MutRangeReqMock
    {
        using TElem = T;
        using TIter = _TIter;
        using TIterEnd = _TIterEnd;
        using TMutIter = _TIter;
        using TMutIterEnd = _TIterEnd;

        TIter Iter() const;
        TIterEnd IterEnd() const;
        TMutIter MutIter();
        TMutIterEnd MutIterEnd();
    };

    /// --------------------------------------------------------------------------------------------
    /// {RRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct RangeReqMock :
        _RangeReqMock<IterReqMock<T>, IterEndReqMock, T> { };

    ATOM_SATISFIES_RANGE_TEMP(RangeReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RMutRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct MutRangeReqMock :
        _MutRangeReqMock<MutIterReqMock<T>, IterEndReqMock, T> { };

    ATOM_SATISFIES_MUT_RANGE_TEMP(MutRangeReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RFwdRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct FwdRangeReqMock :
        _RangeReqMock<FwdIterReqMock<T>, IterEndReqMock, T> { };

    ATOM_SATISFIES_FWD_RANGE_TEMP(FwdRangeReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RMutFwdRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct MutFwdRangeReqMock :
        _MutRangeReqMock<MutFwdIterReqMock<T>, IterEndReqMock, T> { };

    ATOM_SATISFIES_MUT_FWD_RANGE_TEMP(MutFwdRangeReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RBidiRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct BidiRangeReqMock :
        _RangeReqMock<BidiIterReqMock<T>, IterEndReqMock, T> { };

    ATOM_SATISFIES_BIDI_RANGE_TEMP(BidiRangeReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RMutBidiRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct MutBidiRangeReqMock :
        _MutRangeReqMock<MutBidiIterReqMock<T>, IterEndReqMock, T> { };

    ATOM_SATISFIES_MUT_BIDI_RANGE_TEMP(MutBidiRangeReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RJumpRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct JumpRangeReqMock :
        _RangeReqMock<JumpIterReqMock<T>, IterEndReqMock, T> { };

    ATOM_SATISFIES_JUMP_RANGE_TEMP(JumpRangeReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RMutJumpRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct MutJumpRangeReqMock :
        _MutRangeReqMock<MutJumpIterReqMock<T>, IterEndReqMock, T> { };

    ATOM_SATISFIES_MUT_JUMP_RANGE_TEMP(MutJumpRangeReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RArrRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct ArrRangeReqMock :
        _RangeReqMock<ArrIterReqMock<T>, IterEndReqMock, T> { };

    ATOM_SATISFIES_ARR_RANGE_TEMP(ArrRangeReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RMutArrRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct MutArrRangeReqMock :
        _MutRangeReqMock<MutArrIterReqMock<T>, IterEndReqMock, T> { };

    ATOM_SATISFIES_MUT_ARR_RANGE_TEMP(MutArrRangeReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RCommonRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct CommonRangeReqMock : 
        _RangeReqMock<IterReqMock<T>, IterReqMock<T>, T> { };

    ATOM_SATISFIES_COMMON_RANGE_TEMP(CommonRangeReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RMutCommonRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct MutCommonRangeReqMock :
        _MutRangeReqMock<MutFwdIterReqMock<T>, MutFwdIterReqMock<T>, T> { };

    ATOM_SATISFIES_MUT_COMMON_RANGE_TEMP(MutCommonRangeReqMock);
}
