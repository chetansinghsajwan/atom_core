#pragma once
#include "IterReqMocks.h"
#include "RangeReqChecks.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename _TIter, typename _TIterEnd, typename T>
    class _RangeReqMock
    {
    public:
        using TElem = T;
        using TIter = _TIter;
        using TIterEnd = _TIterEnd;

    public:
        auto iter() const -> TIter;
        auto iterEnd() const -> TIterEnd;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    template <typename _TIter, typename _TIterEnd, typename T>
    class _MutRangeReqMock
    {
    public:
        using TElem = T;
        using TIter = _TIter;
        using TIterEnd = _TIterEnd;
        using TMutIter = _TIter;
        using TMutIterEnd = _TIterEnd;

    public:
        auto iter() const -> TIter;
        auto iterEnd() const -> TIterEnd;
        auto mutIter() -> TMutIter;
        auto mutIterEnd() -> TMutIterEnd;
    };

    /// --------------------------------------------------------------------------------------------
    /// `RRange` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class RangeReqMock: public _RangeReqMock<IterReqMock<T>, IterEndReqMock, T>
    {};

    ATOM_SATISFIES_RANGE_TEMP(RangeReqMock);

    /// --------------------------------------------------------------------------------------------
    /// `RMutRange` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class MutRangeReqMock: public _MutRangeReqMock<MutIterReqMock<T>, IterEndReqMock, T>
    {};

    ATOM_SATISFIES_MUT_RANGE_TEMP(MutRangeReqMock);

    /// --------------------------------------------------------------------------------------------
    /// `RFwdRange` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class FwdRangeReqMock: public _RangeReqMock<FwdIterReqMock<T>, IterEndReqMock, T>
    {};

    ATOM_SATISFIES_FWD_RANGE_TEMP(FwdRangeReqMock);

    /// --------------------------------------------------------------------------------------------
    /// `RMutFwdRange` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class MutFwdRangeReqMock: public _MutRangeReqMock<MutFwdIterReqMock<T>, IterEndReqMock, T>
    {};

    ATOM_SATISFIES_MUT_FWD_RANGE_TEMP(MutFwdRangeReqMock);

    /// --------------------------------------------------------------------------------------------
    /// `RBidiRange` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class BidiRangeReqMock: public _RangeReqMock<BidiIterReqMock<T>, IterEndReqMock, T>
    {};

    ATOM_SATISFIES_BIDI_RANGE_TEMP(BidiRangeReqMock);

    /// --------------------------------------------------------------------------------------------
    /// `RMutBidiRange` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class MutBidiRangeReqMock: public _MutRangeReqMock<MutBidiIterReqMock<T>, IterEndReqMock, T>
    {};

    ATOM_SATISFIES_MUT_BIDI_RANGE_TEMP(MutBidiRangeReqMock);

    /// --------------------------------------------------------------------------------------------
    /// `RJumpRange` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class JumpRangeReqMock: public _RangeReqMock<JumpIterReqMock<T>, IterEndReqMock, T>
    {};

    ATOM_SATISFIES_JUMP_RANGE_TEMP(JumpRangeReqMock);

    /// --------------------------------------------------------------------------------------------
    /// `RMutJumpRange` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class MutJumpRangeReqMock: public _MutRangeReqMock<MutJumpIterReqMock<T>, IterEndReqMock, T>
    {};

    ATOM_SATISFIES_MUT_JUMP_RANGE_TEMP(MutJumpRangeReqMock);

    /// --------------------------------------------------------------------------------------------
    /// `RArrayRange` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class ArrayRangeReqMock: public _RangeReqMock<ArrayIterReqMock<T>, ArrayIterReqMock<T>, T>
    {};

    ATOM_SATISFIES_ARR_RANGE_TEMP(ArrayRangeReqMock);

    /// --------------------------------------------------------------------------------------------
    /// `RMutArrayRange` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class MutArrayRangeReqMock: public _MutRangeReqMock<MutArrayIterReqMock<T>, MutArrayIterReqMock<T>, T>
    {};

    ATOM_SATISFIES_MUT_ARR_RANGE_TEMP(MutArrayRangeReqMock);

    /// --------------------------------------------------------------------------------------------
    /// `RCommonRange` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class CommonRangeReqMock: public _RangeReqMock<IterReqMock<T>, IterReqMock<T>, T>
    {};

    ATOM_SATISFIES_COMMON_RANGE_TEMP(CommonRangeReqMock);

    /// --------------------------------------------------------------------------------------------
    /// `RMutCommonRange` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class MutCommonRangeReqMock
        : public _MutRangeReqMock<MutFwdIterReqMock<T>, MutFwdIterReqMock<T>, T>
    {};

    ATOM_SATISFIES_MUT_COMMON_RANGE_TEMP(MutCommonRangeReqMock);
}
