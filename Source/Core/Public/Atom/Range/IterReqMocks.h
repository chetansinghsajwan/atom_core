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

        pub fn op*() const -> const T&;

        pub fn op==(IterEndReqMock end) const -> bool;
        pub fn op!=(IterEndReqMock end) const -> bool;

        pub fn op++(i32) -> IterReqMock&;
    };

    ATOM_SATISFIES_ITER_PAIR(IterReqMock<i32>, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RMutIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    class MutIterReqMock
    {
        pub using TElem = T;

        pub fn op*() -> T&;
        pub fn op*() const -> const T&;

        pub fn op==(IterEndReqMock end) const -> bool;
        pub fn op!=(IterEndReqMock end) const -> bool;

        pub fn op++(i32) -> MutIterReqMock&;
    };

    ATOM_SATISFIES_MUT_ITER_PAIR(MutIterReqMock<i32>, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RFwdIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    class FwdIterReqMock extends MultiPassIterTag
    {
        pub using TElem = T;

        pub fn op*() const -> const T&;

        pub fn op==(IterEndReqMock end) const -> bool;
        pub fn op!=(IterEndReqMock end) const -> bool;

        pub fn op++(i32) -> FwdIterReqMock&;
    };

    ATOM_SATISFIES_FWD_ITER_PAIR(FwdIterReqMock<i32>, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RMutFwdIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    class MutFwdIterReqMock extends MultiPassIterTag
    {
        pub using TElem = T;

        pub fn op*() -> T&;
        pub fn op*() const -> const T&;

        pub fn op==(IterEndReqMock end) const -> bool;
        pub fn op!=(IterEndReqMock end) const -> bool;

        pub fn op++(i32) -> MutFwdIterReqMock&;
    };

    ATOM_SATISFIES_MUT_FWD_ITER_PAIR(MutFwdIterReqMock<i32>, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RBidiIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    class BidiIterReqMock extends MultiPassIterTag
    {
        pub using TElem = T;

        pub fn op*() const -> const T&;

        pub fn op==(IterEndReqMock end) const -> bool;
        pub fn op!=(IterEndReqMock end) const -> bool;

        pub fn op++(i32) -> BidiIterReqMock&;
        pub fn op--(i32) -> BidiIterReqMock&;
    };

    ATOM_SATISFIES_BIDI_ITER_PAIR(BidiIterReqMock<i32>, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RMutBidiIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    class MutBidiIterReqMock extends MultiPassIterTag
    {
        pub using TElem = T;

        pub fn op*() -> T&;
        pub fn op*() const -> const T&;

        pub fn op==(IterEndReqMock end) const -> bool;
        pub fn op!=(IterEndReqMock end) const -> bool;

        pub fn op++(i32) -> MutBidiIterReqMock&;
        pub fn op--(i32) -> MutBidiIterReqMock&;
    };

    ATOM_SATISFIES_MUT_BIDI_ITER_PAIR(MutBidiIterReqMock<i32>, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RJumpIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    class JumpIterReqMock extends MultiPassIterTag
    {
        pub using TElem = T;

        pub fn op*() const -> const T&;

        pub fn op==(IterEndReqMock end) const -> bool;
        pub fn op!=(IterEndReqMock end) const -> bool;

        pub fn op++(i32) -> JumpIterReqMock&;
        pub fn op--(i32) -> JumpIterReqMock&;

        pub fn op+=(usize steps) -> JumpIterReqMock&;
        pub fn op-=(usize steps) -> JumpIterReqMock&;

        pub fn op+(usize steps) const -> JumpIterReqMock;
        pub fn op-(usize steps) const -> JumpIterReqMock;

        pub fn op-(const JumpIterReqMock& that) const -> isize;
    };

    ATOM_SATISFIES_JUMP_ITER_PAIR(JumpIterReqMock<i32>, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RMutJumpIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    class MutJumpIterReqMock extends MultiPassIterTag
    {
        pub using TElem = T;

        pub fn op*() -> T&;
        pub fn op*() const -> const T&;

        pub fn op==(IterEndReqMock end) const -> bool;
        pub fn op!=(IterEndReqMock end) const -> bool;

        pub fn op++(i32) -> MutJumpIterReqMock&;
        pub fn op--(i32) -> MutJumpIterReqMock&;

        pub fn op+=(usize steps) -> MutJumpIterReqMock&;
        pub fn op-=(usize steps) -> MutJumpIterReqMock&;

        pub fn op+(usize steps) const -> MutJumpIterReqMock;
        pub fn op-(usize steps) const -> MutJumpIterReqMock;

        pub fn op-(const MutJumpIterReqMock& that) const -> isize;
    };

    ATOM_SATISFIES_MUT_JUMP_ITER_PAIR(MutJumpIterReqMock<i32>, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RArrIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    class ArrIterReqMock extends ArrIterTag
    {
        pub using TElem = T;

        pub fn op*() -> T&;
        pub fn op*() const -> const T&;

        pub fn op==(ArrIterReqMock end) const -> bool;
        pub fn op!=(ArrIterReqMock end) const -> bool;

        pub fn op++(i32) -> ArrIterReqMock&;
        pub fn op--(i32) -> ArrIterReqMock&;

        pub fn op+=(usize steps) -> ArrIterReqMock&;
        pub fn op-=(usize steps) -> ArrIterReqMock&;

        pub fn op+(usize steps) const -> ArrIterReqMock;
        pub fn op-(usize steps) const -> ArrIterReqMock;

        pub fn op-(const ArrIterReqMock& that) const -> isize;
    };

    ATOM_SATISFIES_ARR_ITER_PAIR(ArrIterReqMock<i32>, ArrIterReqMock<i32>);

    /// --------------------------------------------------------------------------------------------
    /// {RMutArrIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    class MutArrIterReqMock extends ArrIterTag
    {
        pub using TElem = T;

        pub fn op*() -> T&;
        pub fn op*() const -> const T&;

        pub fn op==(MutArrIterReqMock end) const -> bool;
        pub fn op!=(MutArrIterReqMock end) const -> bool;

        pub fn op++(i32) -> MutArrIterReqMock&;
        pub fn op--(i32) -> MutArrIterReqMock&;

        pub fn op+=(usize steps) -> MutArrIterReqMock&;
        pub fn op-=(usize steps) -> MutArrIterReqMock&;

        pub fn op+(usize steps) const -> MutArrIterReqMock;
        pub fn op-(usize steps) const -> MutArrIterReqMock;

        pub fn op-(const MutArrIterReqMock& that) const -> isize;
    };

    ATOM_SATISFIES_MUT_ARR_ITER_PAIR(MutArrIterReqMock<i32>, MutArrIterReqMock<i32>);
}
