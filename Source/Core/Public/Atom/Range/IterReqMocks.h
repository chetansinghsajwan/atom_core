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

        fn operator *() const -> const T&;

        fn operator ==(IterEndReqMock end) const -> bool;
        fn operator !=(IterEndReqMock end) const -> bool;

        fn operator ++(int) -> IterReqMock&;
    };

    ATOM_SATISFIES_ITER_PAIR(IterReqMock<int>, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RMutIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    struct MutIterReqMock
    {
        using TElem = T;

        fn operator *() -> T&;
        fn operator *() const -> const T&;

        fn operator ==(IterEndReqMock end) const -> bool;
        fn operator !=(IterEndReqMock end) const -> bool;

        fn operator ++(int) -> MutIterReqMock&;
    };

    ATOM_SATISFIES_MUT_ITER_PAIR(MutIterReqMock<int>, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RFwdIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    struct FwdIterReqMock : MultiPassIterTag
    {
        using TElem = T;

        fn operator *() const -> const T&;

        fn operator ==(IterEndReqMock end) const -> bool;
        fn operator !=(IterEndReqMock end) const -> bool;

        fn operator ++(int) -> FwdIterReqMock&;
    };

    ATOM_SATISFIES_FWD_ITER_PAIR(FwdIterReqMock<int>, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RMutFwdIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    struct MutFwdIterReqMock : MultiPassIterTag
    {
        using TElem = T;

        fn operator *() -> T&;
        fn operator *() const -> const T&;

        fn operator ==(IterEndReqMock end) const -> bool;
        fn operator !=(IterEndReqMock end) const -> bool;

        fn operator ++(int) -> MutFwdIterReqMock&;
    };

    ATOM_SATISFIES_MUT_FWD_ITER_PAIR(MutFwdIterReqMock<int>, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RBidiIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    struct BidiIterReqMock : MultiPassIterTag
    {
        using TElem = T;

        fn operator *() const -> const T&;

        fn operator ==(IterEndReqMock end) const -> bool;
        fn operator !=(IterEndReqMock end) const -> bool;

        fn operator ++(int) -> BidiIterReqMock&;
        fn operator --(int) -> BidiIterReqMock&;
    };

    ATOM_SATISFIES_BIDI_ITER_PAIR(BidiIterReqMock<int>, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RMutBidiIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    struct MutBidiIterReqMock : MultiPassIterTag
    {
        using TElem = T;

        fn operator *() -> T&;
        fn operator *() const -> const T&;

        fn operator ==(IterEndReqMock end) const -> bool;
        fn operator !=(IterEndReqMock end) const -> bool;

        fn operator ++(int) -> MutBidiIterReqMock&;
        fn operator --(int) -> MutBidiIterReqMock&;
    };

    ATOM_SATISFIES_MUT_BIDI_ITER_PAIR(MutBidiIterReqMock<int>, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RJumpIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    struct JumpIterReqMock : MultiPassIterTag
    {
        using TElem = T;

        fn operator *() const -> const T&;

        fn operator ==(IterEndReqMock end) const -> bool;
        fn operator !=(IterEndReqMock end) const -> bool;

        fn operator ++(int) -> JumpIterReqMock&;
        fn operator --(int) -> JumpIterReqMock&;

        fn operator +=(usize steps) -> JumpIterReqMock&;
        fn operator -=(usize steps) -> JumpIterReqMock&;

        fn operator +(usize steps) const -> JumpIterReqMock;
        fn operator -(usize steps) const -> JumpIterReqMock;

        fn operator -(const JumpIterReqMock& that) const -> isize;
    };

    ATOM_SATISFIES_JUMP_ITER_PAIR(JumpIterReqMock<int>, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RMutJumpIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    struct MutJumpIterReqMock : MultiPassIterTag
    {
        using TElem = T;

        fn operator *() -> T&;
        fn operator *() const -> const T&;

        fn operator ==(IterEndReqMock end) const -> bool;
        fn operator !=(IterEndReqMock end) const -> bool;

        fn operator ++(int) -> MutJumpIterReqMock&;
        fn operator --(int) -> MutJumpIterReqMock&;

        fn operator +=(usize steps) -> MutJumpIterReqMock&;
        fn operator -=(usize steps) -> MutJumpIterReqMock&;

        fn operator +(usize steps) const -> MutJumpIterReqMock;
        fn operator -(usize steps) const -> MutJumpIterReqMock;

        fn operator -(const MutJumpIterReqMock& that) const -> isize;
    };

    ATOM_SATISFIES_MUT_JUMP_ITER_PAIR(MutJumpIterReqMock<int>, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RArrIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    struct ArrIterReqMock : ArrIterTag
    {
        using TElem = T;

        fn operator *() -> T&;
        fn operator *() const -> const T&;

        fn operator ==(IterEndReqMock end) const -> bool;
        fn operator !=(IterEndReqMock end) const -> bool;

        fn operator ++(int) -> ArrIterReqMock&;
        fn operator --(int) -> ArrIterReqMock&;

        fn operator +=(usize steps) -> ArrIterReqMock&;
        fn operator -=(usize steps) -> ArrIterReqMock&;

        fn operator +(usize steps) const -> ArrIterReqMock;
        fn operator -(usize steps) const -> ArrIterReqMock;

        fn operator -(const ArrIterReqMock& that) const -> isize;
    };

    ATOM_SATISFIES_ARR_ITER_PAIR(ArrIterReqMock<int>, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RMutArrIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    struct MutArrIterReqMock : ArrIterTag
    {
        using TElem = T;

        fn operator *() -> T&;
        fn operator *() const -> const T&;

        fn operator ==(IterEndReqMock end) const -> bool;
        fn operator !=(IterEndReqMock end) const -> bool;

        fn operator ++(int) -> MutArrIterReqMock&;
        fn operator --(int) -> MutArrIterReqMock&;

        fn operator +=(usize steps) -> MutArrIterReqMock&;
        fn operator -=(usize steps) -> MutArrIterReqMock&;

        fn operator +(usize steps) const -> MutArrIterReqMock;
        fn operator -(usize steps) const -> MutArrIterReqMock;

        fn operator -(const MutArrIterReqMock& that) const -> isize;
    };

    ATOM_SATISFIES_MUT_ARR_ITER_PAIR(MutArrIterReqMock<int>, IterEndReqMock);
}
