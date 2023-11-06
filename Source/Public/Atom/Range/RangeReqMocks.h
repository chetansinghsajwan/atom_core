#pragma once
#include "IterReqMocks.h"

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

    /// --------------------------------------------------------------------------------------------
    /// `RMutRange` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class MutRangeReqMock: public _MutRangeReqMock<MutIterReqMock<T>, IterEndReqMock, T>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `RFwdRange` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class FwdRangeReqMock: public _RangeReqMock<FwdIterReqMock<T>, IterEndReqMock, T>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `RMutFwdRange` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class MutFwdRangeReqMock: public _MutRangeReqMock<MutFwdIterReqMock<T>, IterEndReqMock, T>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `RBidiRange` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class BidiRangeReqMock: public _RangeReqMock<BidiIterReqMock<T>, IterEndReqMock, T>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `RMutBidiRange` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class MutBidiRangeReqMock: public _MutRangeReqMock<MutBidiIterReqMock<T>, IterEndReqMock, T>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `RJumpRange` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class JumpRangeReqMock: public _RangeReqMock<JumpIterReqMock<T>, IterEndReqMock, T>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `RMutJumpRange` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class MutJumpRangeReqMock: public _MutRangeReqMock<MutJumpIterReqMock<T>, IterEndReqMock, T>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `RArrayRange` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class ArrayRangeReqMock: public _RangeReqMock<ArrayIterReqMock<T>, ArrayIterReqMock<T>, T>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `RMutArrayRange` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class MutArrayRangeReqMock:
        public _MutRangeReqMock<MutArrayIterReqMock<T>, MutArrayIterReqMock<T>, T>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `RCommonRange` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class CommonRangeReqMock: public _RangeReqMock<IterReqMock<T>, IterReqMock<T>, T>
    {};

    /// --------------------------------------------------------------------------------------------
    /// `RMutCommonRange` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class MutCommonRangeReqMock:
        public _MutRangeReqMock<MutFwdIterReqMock<T>, MutFwdIterReqMock<T>, T>
    {};
}
