#pragma once
#include "IterReq.h"

/// ------------------------------------------------------------------------------------------------
/// @TODO[Cpp2PostfixOper]: Remove postfix {++} and {--} operators.
/// ------------------------------------------------------------------------------------------------
namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    struct IterEndReqMock { };

    /// --------------------------------------------------------------------------------------------
    /// {RIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct IterReqMock
    {
        const T& operator *() const;

        bool operator ==(IterEndReqMock end) const;
        bool operator !=(IterEndReqMock end) const;

        IterReqMock& operator ++(int);
    };

    static_assert(RIter<IterReqMock<int>, IterEndReqMock, int>,
        "{IterReqMock} does not meet {RIter} requirements.");

    /// --------------------------------------------------------------------------------------------
    /// {RMutIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct MutIterReqMock
    {
        T& operator *();
        const T& operator *() const;

        bool operator ==(IterEndReqMock end) const;
        bool operator !=(IterEndReqMock end) const;

        MutIterReqMock& operator ++(int);
    };

    static_assert(RMutIter<MutIterReqMock<int>, IterEndReqMock, int>,
        "{MutIterReqMock} does not meet {RMutIter} requirements.");

    /// --------------------------------------------------------------------------------------------
    /// {RFwdIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct FwdIterReqMock : MultiPassIterTag
    {
        const T& operator *() const;

        bool operator ==(IterEndReqMock end) const;
        bool operator !=(IterEndReqMock end) const;

        FwdIterReqMock& operator ++(int);
    };

    static_assert(RFwdIter<FwdIterReqMock<int>, IterEndReqMock, int>,
        "{FwdIterReqMock} does not meet {RFwdIter} requirements.");

    /// --------------------------------------------------------------------------------------------
    /// {RMutFwdIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct MutFwdIterReqMock : MultiPassIterTag
    {
        T& operator *();
        const T& operator *() const;

        bool operator ==(IterEndReqMock end) const;
        bool operator !=(IterEndReqMock end) const;

        MutFwdIterReqMock& operator ++(int);
    };

    static_assert(RMutFwdIter<MutFwdIterReqMock<int>, IterEndReqMock, int>,
        "{MutFwdIterReqMock} does not meet {RMutFwdIter} requirements.");

    /// --------------------------------------------------------------------------------------------
    /// {RBidiIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct BidiIterReqMock : MultiPassIterTag
    {
        const T& operator *() const;

        bool operator ==(IterEndReqMock end) const;
        bool operator !=(IterEndReqMock end) const;

        BidiIterReqMock& operator ++(int);
        BidiIterReqMock& operator --(int);
    };

    static_assert(RBidiIter<BidiIterReqMock<int>, IterEndReqMock, int>,
        "{BidiIterReqMock} does not meet {RBidiIter} requirements.");

    /// --------------------------------------------------------------------------------------------
    /// {RMutBidiIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct MutBidiIterReqMock : MultiPassIterTag
    {
        T& operator *();
        const T& operator *() const;

        bool operator ==(IterEndReqMock end) const;
        bool operator !=(IterEndReqMock end) const;

        MutBidiIterReqMock& operator ++(int);
        MutBidiIterReqMock& operator --(int);
    };

    static_assert(RMutBidiIter<MutBidiIterReqMock<int>, IterEndReqMock, int>,
        "{MutBidiIterReqMock} does not meet {RMutBidiIter} requirements.");

    /// --------------------------------------------------------------------------------------------
    /// {RJumpIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct JumpIterReqMock : MultiPassIterTag
    {
        const T& operator *() const;

        bool operator ==(IterEndReqMock end) const;
        bool operator !=(IterEndReqMock end) const;

        JumpIterReqMock& operator ++(int);
        JumpIterReqMock& operator --(int);

        JumpIterReqMock& operator +=(usize steps);
        JumpIterReqMock& operator -=(usize steps);

        JumpIterReqMock operator +(usize steps) const;
        JumpIterReqMock operator -(usize steps) const;

        isize operator -(const JumpIterReqMock& that) const;
    };

    static_assert(RJumpIter<JumpIterReqMock<int>, IterEndReqMock, int>,
        "{JumpIterReqMock} does not meet {RJumpIter} requirements.");

    /// --------------------------------------------------------------------------------------------
    /// {RMutJumpIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct MutJumpIterReqMock : MultiPassIterTag
    {
        T& operator *();
        const T& operator *() const;

        bool operator ==(IterEndReqMock end) const;
        bool operator !=(IterEndReqMock end) const;

        MutJumpIterReqMock& operator ++(int);
        MutJumpIterReqMock& operator --(int);

        MutJumpIterReqMock& operator +=(usize steps);
        MutJumpIterReqMock& operator -=(usize steps);

        MutJumpIterReqMock operator +(usize steps) const;
        MutJumpIterReqMock operator -(usize steps) const;

        isize operator -(const MutJumpIterReqMock& that) const;
    };

    static_assert(RMutJumpIter<MutJumpIterReqMock<int>, IterEndReqMock, int>,
        "{MutJumpIterReqMock} does not meet {RMutJumpIter} requirements.");

    /// --------------------------------------------------------------------------------------------
    /// {RArrIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct ArrIterReqMock : ArrIterTag
    {
        T& operator *();
        const T& operator *() const;

        bool operator ==(IterEndReqMock end) const;
        bool operator !=(IterEndReqMock end) const;

        ArrIterReqMock& operator ++(int);
        ArrIterReqMock& operator --(int);

        ArrIterReqMock& operator +=(usize steps);
        ArrIterReqMock& operator -=(usize steps);

        ArrIterReqMock operator +(usize steps) const;
        ArrIterReqMock operator -(usize steps) const;

        isize operator -(const ArrIterReqMock& that) const;
    };

    static_assert(RArrIter<ArrIterReqMock<int>, IterEndReqMock, int>,
        "{ArrIterReqMock} does not meet {RArrIter} requirements.");

    /// --------------------------------------------------------------------------------------------
    /// {RMutArrIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct MutArrIterReqMock : ArrIterTag
    {
        T& operator *();
        const T& operator *() const;

        bool operator ==(IterEndReqMock end) const;
        bool operator !=(IterEndReqMock end) const;

        MutArrIterReqMock& operator ++(int);
        MutArrIterReqMock& operator --(int);

        MutArrIterReqMock& operator +=(usize steps);
        MutArrIterReqMock& operator -=(usize steps);

        MutArrIterReqMock operator +(usize steps) const;
        MutArrIterReqMock operator -(usize steps) const;

        isize operator -(const MutArrIterReqMock& that) const;
    };

    static_assert(RMutArrIter<MutArrIterReqMock<int>, IterEndReqMock, int>,
        "{MutArrIterReqMock} does not meet {RMutArrIter} requirements.");
}
