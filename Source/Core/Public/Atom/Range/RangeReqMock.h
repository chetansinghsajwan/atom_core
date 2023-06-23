#pragma once
#include "RangeReq.h"
#include "IterReqMock.h"

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

    static_assert(RRange<RangeReqMock<int>, int>,
        "{RangeReqMock} does not meet {RRange} requirements.");

    /// --------------------------------------------------------------------------------------------
    /// {RMutRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct MutRangeReqMock :
        _MutRangeReqMock<MutIterReqMock<T>, IterEndReqMock, T> { };

    static_assert(RMutRange<MutRangeReqMock<int>, int>,
        "{MutRangeReqMock} does not meet {RMutRange} requirements.");

    /// --------------------------------------------------------------------------------------------
    /// {RFwdRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct FwdRangeReqMock :
        _RangeReqMock<FwdIterReqMock<T>, IterEndReqMock, T> { };

    static_assert(RFwdRange<FwdRangeReqMock<int>, int>,
        "{FwdRangeReqMock} does not meet {RFwdRange} requirements.");

    /// --------------------------------------------------------------------------------------------
    /// {RMutFwdRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct MutFwdRangeReqMock :
        _MutRangeReqMock<MutFwdIterReqMock<T>, IterEndReqMock, T> { };

    static_assert(RFwdRange<FwdRangeReqMock<int>, int>,
        "{MutFwdRangeReqMock} does not meet {RMutFwdRange} requirements.");

    /// --------------------------------------------------------------------------------------------
    /// {RBidiRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct BidiRangeReqMock :
        _RangeReqMock<BidiIterReqMock<T>, IterEndReqMock, T> { };

    static_assert(RBidiRange<BidiRangeReqMock<int>, int>,
        "{BidiRangeReqMock} does not meet {RBidiRange} requirements.");

    /// --------------------------------------------------------------------------------------------
    /// {RMutBidiRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct MutBidiRangeReqMock :
        _MutRangeReqMock<MutBidiIterReqMock<T>, IterEndReqMock, T> { };

    static_assert(RBidiRange<BidiRangeReqMock<int>, int>,
        "{MutBidiRangeReqMock} does not meet {RMutBidiRange} requirements.");

    /// --------------------------------------------------------------------------------------------
    /// {RJumpRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct JumpRangeReqMock :
        _RangeReqMock<JumpIterReqMock<T>, IterEndReqMock, T> { };

    static_assert(RJumpRange<JumpRangeReqMock<int>, int>,
        "{JumpRangeReqMock} does not meet {RJumpRange} requirements.");

    /// --------------------------------------------------------------------------------------------
    /// {RMutJumpRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct MutJumpRangeReqMock :
        _MutRangeReqMock<MutJumpIterReqMock<T>, IterEndReqMock, T> { };

    static_assert(RJumpRange<JumpRangeReqMock<int>, int>,
        "{MutJumpRangeReqMock} does not meet {RMutJumpRange} requirements.");

    /// --------------------------------------------------------------------------------------------
    /// {RArrRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct ArrRangeReqMock :
        _RangeReqMock<ArrIterReqMock<T>, IterEndReqMock, T> { };

    static_assert(RArrRange<ArrRangeReqMock<int>, int>,
        "{ArrRangeReqMock} does not meet {RArrRange} requirements.");

    /// --------------------------------------------------------------------------------------------
    /// {RMutArrRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct MutArrRangeReqMock :
        _MutRangeReqMock<MutArrIterReqMock<T>, IterEndReqMock, T> { };

    static_assert(RArrRange<ArrRangeReqMock<int>, int>,
        "{MutArrRangeReqMock} does not meet {RMutArrRange} requirements.");

    /// --------------------------------------------------------------------------------------------
    /// {RCommonRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct CommonRangeReqMock : 
        _RangeReqMock<IterReqMock<T>, IterReqMock<T>, T> { };

    static_assert(RCommonRange<CommonRangeReqMock<int>, int>,
        "{CommonRangeReqMock} does not meet {RCommonRange} requirements.");

    /// --------------------------------------------------------------------------------------------
    /// {RMutCommonRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct MutCommonRangeReqMock :
        _MutRangeReqMock<MutFwdIterReqMock<T>, MutFwdIterReqMock<T>, T> { };

    static_assert(RCommonRange<CommonRangeReqMock<int>, int>,
        "{MutCommonRangeReqMock} does not meet {RMutCommonRange} requirements.");
}
