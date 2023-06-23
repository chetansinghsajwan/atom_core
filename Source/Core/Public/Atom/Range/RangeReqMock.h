#pragma once
#include "RangeReq.h"
#include "IterReqMock.h"

/// ------------------------------------------------------------------------------------------------
/// Macro to check if range {range} satisfies range req {req}.
/// ------------------------------------------------------------------------------------------------
#define ATOM_STATIC_ASSERT_RANGE_REQ(req, range) \
    static_assert(req<range<int>, int>, \
        "{" #range "} does not satisfy {" #req "} requirements.");

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

    ATOM_STATIC_ASSERT_RANGE_REQ(RRangeOf, RangeReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RMutRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct MutRangeReqMock :
        _MutRangeReqMock<MutIterReqMock<T>, IterEndReqMock, T> { };

    ATOM_STATIC_ASSERT_RANGE_REQ(RMutRangeOf, MutRangeReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RFwdRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct FwdRangeReqMock :
        _RangeReqMock<FwdIterReqMock<T>, IterEndReqMock, T> { };

    ATOM_STATIC_ASSERT_RANGE_REQ(RFwdRangeOf, FwdRangeReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RMutFwdRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct MutFwdRangeReqMock :
        _MutRangeReqMock<MutFwdIterReqMock<T>, IterEndReqMock, T> { };

    ATOM_STATIC_ASSERT_RANGE_REQ(RMutFwdRangeOf, MutFwdRangeReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RBidiRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct BidiRangeReqMock :
        _RangeReqMock<BidiIterReqMock<T>, IterEndReqMock, T> { };

    ATOM_STATIC_ASSERT_RANGE_REQ(RBidiRangeOf, BidiRangeReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RMutBidiRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct MutBidiRangeReqMock :
        _MutRangeReqMock<MutBidiIterReqMock<T>, IterEndReqMock, T> { };

    ATOM_STATIC_ASSERT_RANGE_REQ(RMutBidiRangeOf, MutBidiRangeReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RJumpRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct JumpRangeReqMock :
        _RangeReqMock<JumpIterReqMock<T>, IterEndReqMock, T> { };

    ATOM_STATIC_ASSERT_RANGE_REQ(RJumpRangeOf, JumpRangeReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RMutJumpRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct MutJumpRangeReqMock :
        _MutRangeReqMock<MutJumpIterReqMock<T>, IterEndReqMock, T> { };

    ATOM_STATIC_ASSERT_RANGE_REQ(RMutJumpRangeOf, MutJumpRangeReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RArrRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct ArrRangeReqMock :
        _RangeReqMock<ArrIterReqMock<T>, IterEndReqMock, T> { };

    ATOM_STATIC_ASSERT_RANGE_REQ(RArrRangeOf, ArrRangeReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RMutArrRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct MutArrRangeReqMock :
        _MutRangeReqMock<MutArrIterReqMock<T>, IterEndReqMock, T> { };

    ATOM_STATIC_ASSERT_RANGE_REQ(RMutArrRangeOf, MutArrRangeReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RCommonRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct CommonRangeReqMock : 
        _RangeReqMock<IterReqMock<T>, IterReqMock<T>, T> { };

    ATOM_STATIC_ASSERT_RANGE_REQ(RCommonRangeOf, CommonRangeReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RMutCommonRange} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct MutCommonRangeReqMock :
        _MutRangeReqMock<MutFwdIterReqMock<T>, MutFwdIterReqMock<T>, T> { };

    ATOM_STATIC_ASSERT_RANGE_REQ(RMutCommonRangeOf, MutCommonRangeReqMock);
}
