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
    class IterEndReqMock { };

    /// --------------------------------------------------------------------------------------------
    /// {RIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    class IterReqMock
    {
        pub using TElem = T;

        pub fn operator *() const -> const T&;

        pub fn operator ==(IterEndReqMock end) const -> bool;
        pub fn operator !=(IterEndReqMock end) const -> bool;

        pub fn operator ++(int) -> IterReqMock&;
    };

    ATOM_SATISFIES_ITER_PAIR(IterReqMock<int>, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RMutIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    class MutIterReqMock
    {
        pub using TElem = T;

        pub fn operator *() -> T&;
        pub fn operator *() const -> const T&;

        pub fn operator ==(IterEndReqMock end) const -> bool;
        pub fn operator !=(IterEndReqMock end) const -> bool;

        pub fn operator ++(int) -> MutIterReqMock&;
    };

    ATOM_SATISFIES_MUT_ITER_PAIR(MutIterReqMock<int>, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RFwdIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    class FwdIterReqMock extends MultiPassIterTag
    {
        pub using TElem = T;

        pub fn operator *() const -> const T&;

        pub fn operator ==(IterEndReqMock end) const -> bool;
        pub fn operator !=(IterEndReqMock end) const -> bool;

        pub fn operator ++(int) -> FwdIterReqMock&;
    };

    ATOM_SATISFIES_FWD_ITER_PAIR(FwdIterReqMock<int>, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RMutFwdIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    class MutFwdIterReqMock extends MultiPassIterTag
    {
        pub using TElem = T;

        pub fn operator *() -> T&;
        pub fn operator *() const -> const T&;

        pub fn operator ==(IterEndReqMock end) const -> bool;
        pub fn operator !=(IterEndReqMock end) const -> bool;

        pub fn operator ++(int) -> MutFwdIterReqMock&;
    };

    ATOM_SATISFIES_MUT_FWD_ITER_PAIR(MutFwdIterReqMock<int>, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RBidiIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    class BidiIterReqMock extends MultiPassIterTag
    {
        pub using TElem = T;

        pub fn operator *() const -> const T&;

        pub fn operator ==(IterEndReqMock end) const -> bool;
        pub fn operator !=(IterEndReqMock end) const -> bool;

        pub fn operator ++(int) -> BidiIterReqMock&;
        pub fn operator --(int) -> BidiIterReqMock&;
    };

    ATOM_SATISFIES_BIDI_ITER_PAIR(BidiIterReqMock<int>, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RMutBidiIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    class MutBidiIterReqMock extends MultiPassIterTag
    {
        pub using TElem = T;

        pub fn operator *() -> T&;
        pub fn operator *() const -> const T&;

        pub fn operator ==(IterEndReqMock end) const -> bool;
        pub fn operator !=(IterEndReqMock end) const -> bool;

        pub fn operator ++(int) -> MutBidiIterReqMock&;
        pub fn operator --(int) -> MutBidiIterReqMock&;
    };

    ATOM_SATISFIES_MUT_BIDI_ITER_PAIR(MutBidiIterReqMock<int>, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RJumpIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    class JumpIterReqMock extends MultiPassIterTag
    {
        pub using TElem = T;

        pub fn operator *() const -> const T&;

        pub fn operator ==(IterEndReqMock end) const -> bool;
        pub fn operator !=(IterEndReqMock end) const -> bool;

        pub fn operator ++(int) -> JumpIterReqMock&;
        pub fn operator --(int) -> JumpIterReqMock&;

        pub fn operator +=(usize steps) -> JumpIterReqMock&;
        pub fn operator -=(usize steps) -> JumpIterReqMock&;

        pub fn operator +(usize steps) const -> JumpIterReqMock;
        pub fn operator -(usize steps) const -> JumpIterReqMock;

        pub fn operator -(const JumpIterReqMock& that) const -> isize;
    };

    ATOM_SATISFIES_JUMP_ITER_PAIR(JumpIterReqMock<int>, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RMutJumpIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    class MutJumpIterReqMock extends MultiPassIterTag
    {
        pub using TElem = T;

        pub fn operator *() -> T&;
        pub fn operator *() const -> const T&;

        pub fn operator ==(IterEndReqMock end) const -> bool;
        pub fn operator !=(IterEndReqMock end) const -> bool;

        pub fn operator ++(int) -> MutJumpIterReqMock&;
        pub fn operator --(int) -> MutJumpIterReqMock&;

        pub fn operator +=(usize steps) -> MutJumpIterReqMock&;
        pub fn operator -=(usize steps) -> MutJumpIterReqMock&;

        pub fn operator +(usize steps) const -> MutJumpIterReqMock;
        pub fn operator -(usize steps) const -> MutJumpIterReqMock;

        pub fn operator -(const MutJumpIterReqMock& that) const -> isize;
    };

    ATOM_SATISFIES_MUT_JUMP_ITER_PAIR(MutJumpIterReqMock<int>, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RArrIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    class ArrIterReqMock extends ArrIterTag
    {
        pub using TElem = T;

        pub fn operator *() -> T&;
        pub fn operator *() const -> const T&;

        pub fn operator ==(ArrIterReqMock end) const -> bool;
        pub fn operator !=(ArrIterReqMock end) const -> bool;

        pub fn operator ++(int) -> ArrIterReqMock&;
        pub fn operator --(int) -> ArrIterReqMock&;

        pub fn operator +=(usize steps) -> ArrIterReqMock&;
        pub fn operator -=(usize steps) -> ArrIterReqMock&;

        pub fn operator +(usize steps) const -> ArrIterReqMock;
        pub fn operator -(usize steps) const -> ArrIterReqMock;

        pub fn operator -(const ArrIterReqMock& that) const -> isize;
    };

    ATOM_SATISFIES_ARR_ITER_PAIR(ArrIterReqMock<int>, ArrIterReqMock<int>);

    /// --------------------------------------------------------------------------------------------
    /// {RMutArrIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    class MutArrIterReqMock extends ArrIterTag
    {
        pub using TElem = T;

        pub fn operator *() -> T&;
        pub fn operator *() const -> const T&;

        pub fn operator ==(MutArrIterReqMock end) const -> bool;
        pub fn operator !=(MutArrIterReqMock end) const -> bool;

        pub fn operator ++(int) -> MutArrIterReqMock&;
        pub fn operator --(int) -> MutArrIterReqMock&;

        pub fn operator +=(usize steps) -> MutArrIterReqMock&;
        pub fn operator -=(usize steps) -> MutArrIterReqMock&;

        pub fn operator +(usize steps) const -> MutArrIterReqMock;
        pub fn operator -(usize steps) const -> MutArrIterReqMock;

        pub fn operator -(const MutArrIterReqMock& that) const -> isize;
    };

    ATOM_SATISFIES_MUT_ARR_ITER_PAIR(MutArrIterReqMock<int>, MutArrIterReqMock<int>);
}
