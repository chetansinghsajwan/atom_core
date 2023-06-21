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
    template <typename _TMutIter, typename _TMutIterEnd, typename T>
    struct _MutRangeReqMock
    {
        using TElem = T;
        using TMutIter = _TMutIter;
        using TMutIterEnd = _TMutIterEnd;

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
    /// {RFwdRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct FwdRangeReqMock : MultiPassIterTag,
        _RangeReqMock<FwdIterReqMock<T>, IterEndReqMock, T> { };

    static_assert(RFwdRange<FwdRangeReqMock<int>, int>,
        "{FwdRangeReqMock} does not meet {RFwdRange} requirements.");

    /// --------------------------------------------------------------------------------------------
    /// {RMutFwdRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct MutFwdRangeReqMock : FwdRangeReqMock<T>,
        _MutRangeReqMock<MutFwdIterReqMock<T>, IterEndReqMock, T> { };

    static_assert(RFwdRange<FwdRangeReqMock<int>, int>,
        "{MutFwdRangeReqMock} does not meet {RMutFwdRange} requirements.");

    /// --------------------------------------------------------------------------------------------
    /// {RBidiRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct BidiRangeReqMock : MultiPassIterTag,
        _RangeReqMock<BidiIterReqMock<T>, IterEndReqMock, T> { };

    static_assert(RBidiRange<BidiRangeReqMock<int>, int>,
        "{BidiRangeReqMock} does not meet {RBidiRange} requirements.");

    /// --------------------------------------------------------------------------------------------
    /// {RMutBidiRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct MutBidiRangeReqMock : BidiRangeReqMock<T>,
        _MutRangeReqMock<MutBidiIterReqMock<T>, IterEndReqMock, T> { };

    static_assert(RBidiRange<BidiRangeReqMock<int>, int>,
        "{MutBidiRangeReqMock} does not meet {RMutBidiRange} requirements.");

    /// --------------------------------------------------------------------------------------------
    /// {RJumpRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct JumpRangeReqMock : MultiPassIterTag,
        _RangeReqMock<JumpIterReqMock<T>, IterEndReqMock, T> { };

    static_assert(RJumpRange<JumpRangeReqMock<int>, int>,
        "{JumpRangeReqMock} does not meet {RJumpRange} requirements.");

    /// --------------------------------------------------------------------------------------------
    /// {RMutJumpRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct MutJumpRangeReqMock : JumpRangeReqMock<T>,
        _MutRangeReqMock<MutJumpIterReqMock<T>, IterEndReqMock, T> { };

    static_assert(RJumpRange<JumpRangeReqMock<int>, int>,
        "{MutJumpRangeReqMock} does not meet {RMutJumpRange} requirements.");

    /// --------------------------------------------------------------------------------------------
    /// {RArrRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct ArrRangeReqMock : MultiPassIterTag,
        _RangeReqMock<ArrIterReqMock<T>, IterEndReqMock, T> { };

    static_assert(RArrRange<ArrRangeReqMock<int>, int>,
        "{ArrRangeReqMock} does not meet {RArrRange} requirements.");

    /// --------------------------------------------------------------------------------------------
    /// {RMutArrRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct MutArrRangeReqMock : ArrRangeReqMock<T>,
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
    struct MutCommonRangeReqMock : CommonRangeReqMock<T>,
        _MutRangeReqMock<MutFwdIterReqMock<T>, MutFwdIterReqMock<T>, T> { };

    static_assert(RCommonRange<CommonRangeReqMock<int>, int>,
        "{MutCommonRangeReqMock} does not meet {RMutCommonRange} requirements.");
}
