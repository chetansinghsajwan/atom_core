#pragma once
#include "IterReq.h"

/// ------------------------------------------------------------------------------------------------
/// Macro to check if iter {iter} satisfies iter req {req} with end {iterEnd}.
/// ------------------------------------------------------------------------------------------------
#define ATOM_STATIC_ASSERT_ITER_REQ(req, iter, iterEnd) \
    static_assert(req<iter<int>, iterEnd, int>, \
        "{" #iter ", " #iterEnd "} does not satisfy {" #req "} requirements.");

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
    template <typename T>
    struct IterReqMock
    {
        using TElem = T;

        const T& operator *() const;

        bool operator ==(IterEndReqMock end) const;
        bool operator !=(IterEndReqMock end) const;

        IterReqMock& operator ++(int);
    };

    ATOM_STATIC_ASSERT_ITER_REQ(RIterPairOf, IterReqMock, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RMutIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct MutIterReqMock
    {
        using TElem = T;

        T& operator *();
        const T& operator *() const;

        bool operator ==(IterEndReqMock end) const;
        bool operator !=(IterEndReqMock end) const;

        MutIterReqMock& operator ++(int);
    };

    ATOM_STATIC_ASSERT_ITER_REQ(RMutIterPairOf, MutIterReqMock, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RFwdIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct FwdIterReqMock : MultiPassIterTag
    {
        using TElem = T;

        const T& operator *() const;

        bool operator ==(IterEndReqMock end) const;
        bool operator !=(IterEndReqMock end) const;

        FwdIterReqMock& operator ++(int);
    };

    ATOM_STATIC_ASSERT_ITER_REQ(RFwdIterPairOf, FwdIterReqMock, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RMutFwdIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct MutFwdIterReqMock : MultiPassIterTag
    {
        using TElem = T;

        T& operator *();
        const T& operator *() const;

        bool operator ==(IterEndReqMock end) const;
        bool operator !=(IterEndReqMock end) const;

        MutFwdIterReqMock& operator ++(int);
    };

    ATOM_STATIC_ASSERT_ITER_REQ(RMutFwdIterPairOf, MutFwdIterReqMock, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RBidiIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct BidiIterReqMock : MultiPassIterTag
    {
        using TElem = T;

        const T& operator *() const;

        bool operator ==(IterEndReqMock end) const;
        bool operator !=(IterEndReqMock end) const;

        BidiIterReqMock& operator ++(int);
        BidiIterReqMock& operator --(int);
    };

    ATOM_STATIC_ASSERT_ITER_REQ(RBidiIterPairOf, BidiIterReqMock, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RMutBidiIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
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

    ATOM_STATIC_ASSERT_ITER_REQ(RMutBidiIterPairOf, MutBidiIterReqMock, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RJumpIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
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

    ATOM_STATIC_ASSERT_ITER_REQ(RJumpIterPairOf, JumpIterReqMock, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RMutJumpIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
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

    ATOM_STATIC_ASSERT_ITER_REQ(RMutJumpIterPairOf, MutJumpIterReqMock, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RArrIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
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

    ATOM_STATIC_ASSERT_ITER_REQ(RArrIterPairOf, ArrIterReqMock, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RMutArrIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
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

    ATOM_STATIC_ASSERT_ITER_REQ(RMutArrIterPairOf, MutArrIterReqMock, IterEndReqMock);
}
