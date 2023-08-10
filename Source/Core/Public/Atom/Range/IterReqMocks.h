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
    template <typename T>
    class IterReqMock
    {
        pub using TElem = T;

        pub fn operator*() const -> const T&;

        pub fn operator==(IterEndReqMock end) const -> bool;
        pub fn operator!=(IterEndReqMock end) const -> bool;

        pub fn operator++(i32) -> IterReqMock&;
    };

    ATOM_SATISFIES_ITER_PAIR(IterReqMock<i32>, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RMutIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class MutIterReqMock
    {
        pub using TElem = T;

        pub fn operator*() -> T&;
        pub fn operator*() const -> const T&;

        pub fn operator==(IterEndReqMock end) const -> bool;
        pub fn operator!=(IterEndReqMock end) const -> bool;

        pub fn operator++(i32) -> MutIterReqMock&;
    };

    ATOM_SATISFIES_MUT_ITER_PAIR(MutIterReqMock<i32>, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RFwdIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class FwdIterReqMock extends MultiPassIterTag
    {
        pub using TElem = T;

        pub fn operator*() const -> const T&;

        pub fn operator==(IterEndReqMock end) const -> bool;
        pub fn operator!=(IterEndReqMock end) const -> bool;

        pub fn operator++(i32) -> FwdIterReqMock&;
    };

    ATOM_SATISFIES_FWD_ITER_PAIR(FwdIterReqMock<i32>, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RMutFwdIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class MutFwdIterReqMock extends MultiPassIterTag
    {
        pub using TElem = T;

        pub fn operator*() -> T&;
        pub fn operator*() const -> const T&;

        pub fn operator==(IterEndReqMock end) const -> bool;
        pub fn operator!=(IterEndReqMock end) const -> bool;

        pub fn operator++(i32) -> MutFwdIterReqMock&;
    };

    ATOM_SATISFIES_MUT_FWD_ITER_PAIR(MutFwdIterReqMock<i32>, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RBidiIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class BidiIterReqMock extends MultiPassIterTag
    {
        pub using TElem = T;

        pub fn operator*() const -> const T&;

        pub fn operator==(IterEndReqMock end) const -> bool;
        pub fn operator!=(IterEndReqMock end) const -> bool;

        pub fn operator++(i32) -> BidiIterReqMock&;
        pub fn operator--(i32) -> BidiIterReqMock&;
    };

    ATOM_SATISFIES_BIDI_ITER_PAIR(BidiIterReqMock<i32>, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RMutBidiIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class MutBidiIterReqMock extends MultiPassIterTag
    {
        pub using TElem = T;

        pub fn operator*() -> T&;
        pub fn operator*() const -> const T&;

        pub fn operator==(IterEndReqMock end) const -> bool;
        pub fn operator!=(IterEndReqMock end) const -> bool;

        pub fn operator++(i32) -> MutBidiIterReqMock&;
        pub fn operator--(i32) -> MutBidiIterReqMock&;
    };

    ATOM_SATISFIES_MUT_BIDI_ITER_PAIR(MutBidiIterReqMock<i32>, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RJumpIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class JumpIterReqMock extends MultiPassIterTag
    {
        pub using TElem = T;

        pub fn operator*() const -> const T&;

        pub fn operator==(IterEndReqMock end) const -> bool;
        pub fn operator!=(IterEndReqMock end) const -> bool;

        pub fn operator++(i32) -> JumpIterReqMock&;
        pub fn operator--(i32) -> JumpIterReqMock&;

        pub fn operator+=(usize steps) -> JumpIterReqMock&;
        pub fn operator-=(usize steps) -> JumpIterReqMock&;

        pub fn operator+(usize steps) const -> JumpIterReqMock;
        pub fn operator-(usize steps) const -> JumpIterReqMock;

        pub fn operator-(const JumpIterReqMock& that) const -> isize;
    };

    ATOM_SATISFIES_JUMP_ITER_PAIR(JumpIterReqMock<i32>, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RMutJumpIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class MutJumpIterReqMock extends MultiPassIterTag
    {
        pub using TElem = T;

        pub fn operator*() -> T&;
        pub fn operator*() const -> const T&;

        pub fn operator==(IterEndReqMock end) const -> bool;
        pub fn operator!=(IterEndReqMock end) const -> bool;

        pub fn operator++(i32) -> MutJumpIterReqMock&;
        pub fn operator--(i32) -> MutJumpIterReqMock&;

        pub fn operator+=(usize steps) -> MutJumpIterReqMock&;
        pub fn operator-=(usize steps) -> MutJumpIterReqMock&;

        pub fn operator+(usize steps) const -> MutJumpIterReqMock;
        pub fn operator-(usize steps) const -> MutJumpIterReqMock;

        pub fn operator-(const MutJumpIterReqMock& that) const -> isize;
    };

    ATOM_SATISFIES_MUT_JUMP_ITER_PAIR(MutJumpIterReqMock<i32>, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RArrIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class ArrIterReqMock extends ArrIterTag
    {
        pub using TElem = T;

        pub fn operator*() -> T&;
        pub fn operator*() const -> const T&;

        pub fn operator==(ArrIterReqMock end) const -> bool;
        pub fn operator!=(ArrIterReqMock end) const -> bool;

        pub fn operator++(i32) -> ArrIterReqMock&;
        pub fn operator--(i32) -> ArrIterReqMock&;

        pub fn operator+=(usize steps) -> ArrIterReqMock&;
        pub fn operator-=(usize steps) -> ArrIterReqMock&;

        pub fn operator+(usize steps) const -> ArrIterReqMock;
        pub fn operator-(usize steps) const -> ArrIterReqMock;

        pub fn operator-(const ArrIterReqMock& that) const -> isize;
    };

    ATOM_SATISFIES_ARR_ITER_PAIR(ArrIterReqMock<i32>, ArrIterReqMock<i32>);

    /// --------------------------------------------------------------------------------------------
    /// {RMutArrIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class MutArrIterReqMock extends ArrIterTag
    {
        pub using TElem = T;

        pub fn operator*() -> T&;
        pub fn operator*() const -> const T&;

        pub fn operator==(MutArrIterReqMock end) const -> bool;
        pub fn operator!=(MutArrIterReqMock end) const -> bool;

        pub fn operator++(i32) -> MutArrIterReqMock&;
        pub fn operator--(i32) -> MutArrIterReqMock&;

        pub fn operator+=(usize steps) -> MutArrIterReqMock&;
        pub fn operator-=(usize steps) -> MutArrIterReqMock&;

        pub fn operator+(usize steps) const -> MutArrIterReqMock;
        pub fn operator-(usize steps) const -> MutArrIterReqMock;

        pub fn operator-(const MutArrIterReqMock& that) const -> isize;
    };

    ATOM_SATISFIES_MUT_ARR_ITER_PAIR(MutArrIterReqMock<i32>, MutArrIterReqMock<i32>);
}
