#pragma once
#include "IterReqChecks.h"

/// ------------------------------------------------------------------------------------------------
/// @TODO[Cpp2PostfixOper]: Remove postfix {++} and {--} operators.
/// ------------------------------------------------------------------------------------------------
namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// Mock object used as IterEnd.
    /// --------------------------------------------------------------------------------------------
    struct IterEndReqMock { };

    /// --------------------------------------------------------------------------------------------
    /// {RIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    struct IterReqMock
    {
        using TElem = T;

        const T& operator *() const;

        bool operator ==(IterEndReqMock end) const;
        bool operator !=(IterEndReqMock end) const;

        IterReqMock& operator ++(int);
    };

    ATOM_SATISFIES_ITER_PAIR(IterReqMock<int>, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RMutIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    struct MutIterReqMock
    {
        using TElem = T;

        T& operator *();
        const T& operator *() const;

        bool operator ==(IterEndReqMock end) const;
        bool operator !=(IterEndReqMock end) const;

        MutIterReqMock& operator ++(int);
    };

    ATOM_SATISFIES_MUT_ITER_PAIR(MutIterReqMock<int>, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RFwdIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    struct FwdIterReqMock : MultiPassIterTag
    {
        using TElem = T;

        const T& operator *() const;

        bool operator ==(IterEndReqMock end) const;
        bool operator !=(IterEndReqMock end) const;

        FwdIterReqMock& operator ++(int);
    };

    ATOM_SATISFIES_FWD_ITER_PAIR(FwdIterReqMock<int>, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RMutFwdIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    struct MutFwdIterReqMock : MultiPassIterTag
    {
        using TElem = T;

        T& operator *();
        const T& operator *() const;

        bool operator ==(IterEndReqMock end) const;
        bool operator !=(IterEndReqMock end) const;

        MutFwdIterReqMock& operator ++(int);
    };

    ATOM_SATISFIES_MUT_FWD_ITER_PAIR(MutFwdIterReqMock<int>, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RBidiIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    struct BidiIterReqMock : MultiPassIterTag
    {
        using TElem = T;

        const T& operator *() const;

        bool operator ==(IterEndReqMock end) const;
        bool operator !=(IterEndReqMock end) const;

        BidiIterReqMock& operator ++(int);
        BidiIterReqMock& operator --(int);
    };

    ATOM_SATISFIES_BIDI_ITER_PAIR(BidiIterReqMock<int>, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RMutBidiIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    struct MutBidiIterReqMock : MultiPassIterTag
    {
        using TElem = T;

        T& operator *();
        const T& operator *() const;

        bool operator ==(IterEndReqMock end) const;
        bool operator !=(IterEndReqMock end) const;

        MutBidiIterReqMock& operator ++(int);
        MutBidiIterReqMock& operator --(int);
    };

    ATOM_SATISFIES_MUT_BIDI_ITER_PAIR(MutBidiIterReqMock<int>, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RJumpIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    struct JumpIterReqMock : MultiPassIterTag
    {
        using TElem = T;

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

    ATOM_SATISFIES_JUMP_ITER_PAIR(JumpIterReqMock<int>, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RMutJumpIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    struct MutJumpIterReqMock : MultiPassIterTag
    {
        using TElem = T;

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

    ATOM_SATISFIES_MUT_JUMP_ITER_PAIR(MutJumpIterReqMock<int>, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RArrIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    struct ArrIterReqMock : ArrIterTag
    {
        using TElem = T;

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

    ATOM_SATISFIES_ARR_ITER_PAIR(ArrIterReqMock<int>, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RMutArrIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    struct MutArrIterReqMock : ArrIterTag
    {
        using TElem = T;

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

    ATOM_SATISFIES_MUT_ARR_ITER_PAIR(MutArrIterReqMock<int>, IterEndReqMock);
}
