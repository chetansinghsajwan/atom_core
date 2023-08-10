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
    public:
        using TElem = T;

    public:
        auto operator*() const -> const T&;

        auto operator==(IterEndReqMock end) const -> bool;
        auto operator!=(IterEndReqMock end) const -> bool;

        auto operator++(i32) -> IterReqMock&;
    };

    ATOM_SATISFIES_ITER_PAIR(IterReqMock<i32>, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RMutIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class MutIterReqMock
    {
    public:
        using TElem = T;

    public:
        auto operator*() -> T&;
        auto operator*() const -> const T&;

        auto operator==(IterEndReqMock end) const -> bool;
        auto operator!=(IterEndReqMock end) const -> bool;

        auto operator++(i32) -> MutIterReqMock&;
    };

    ATOM_SATISFIES_MUT_ITER_PAIR(MutIterReqMock<i32>, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RFwdIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class FwdIterReqMock : public MultiPassIterTag
    {
    public:
        using TElem = T;

    public:
        auto operator*() const -> const T&;

        auto operator==(IterEndReqMock end) const -> bool;
        auto operator!=(IterEndReqMock end) const -> bool;

        auto operator++(i32) -> FwdIterReqMock&;
    };

    ATOM_SATISFIES_FWD_ITER_PAIR(FwdIterReqMock<i32>, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RMutFwdIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class MutFwdIterReqMock : public MultiPassIterTag
    {
    public:
        using TElem = T;

    public:
        auto operator*() -> T&;
        auto operator*() const -> const T&;

        auto operator==(IterEndReqMock end) const -> bool;
        auto operator!=(IterEndReqMock end) const -> bool;

        auto operator++(i32) -> MutFwdIterReqMock&;
    };

    ATOM_SATISFIES_MUT_FWD_ITER_PAIR(MutFwdIterReqMock<i32>, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RBidiIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class BidiIterReqMock : public MultiPassIterTag
    {
    public:
        using TElem = T;

    public:
        auto operator*() const -> const T&;

        auto operator==(IterEndReqMock end) const -> bool;
        auto operator!=(IterEndReqMock end) const -> bool;

        auto operator++(i32) -> BidiIterReqMock&;
        auto operator--(i32) -> BidiIterReqMock&;
    };

    ATOM_SATISFIES_BIDI_ITER_PAIR(BidiIterReqMock<i32>, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RMutBidiIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class MutBidiIterReqMock : public MultiPassIterTag
    {
    public:
        using TElem = T;

    public:
        auto operator*() -> T&;
        auto operator*() const -> const T&;

        auto operator==(IterEndReqMock end) const -> bool;
        auto operator!=(IterEndReqMock end) const -> bool;

        auto operator++(i32) -> MutBidiIterReqMock&;
        auto operator--(i32) -> MutBidiIterReqMock&;
    };

    ATOM_SATISFIES_MUT_BIDI_ITER_PAIR(MutBidiIterReqMock<i32>, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RJumpIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class JumpIterReqMock : public MultiPassIterTag
    {
    public:
        using TElem = T;

    public:
        auto operator*() const -> const T&;

        auto operator==(IterEndReqMock end) const -> bool;
        auto operator!=(IterEndReqMock end) const -> bool;

        auto operator++(i32) -> JumpIterReqMock&;
        auto operator--(i32) -> JumpIterReqMock&;

        auto operator+=(usize steps) -> JumpIterReqMock&;
        auto operator-=(usize steps) -> JumpIterReqMock&;

        auto operator+(usize steps) const -> JumpIterReqMock;
        auto operator-(usize steps) const -> JumpIterReqMock;

        auto operator-(const JumpIterReqMock& that) const -> isize;
    };

    ATOM_SATISFIES_JUMP_ITER_PAIR(JumpIterReqMock<i32>, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RMutJumpIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class MutJumpIterReqMock : public MultiPassIterTag
    {
    public:
        using TElem = T;

    public:
        auto operator*() -> T&;
        auto operator*() const -> const T&;

        auto operator==(IterEndReqMock end) const -> bool;
        auto operator!=(IterEndReqMock end) const -> bool;

        auto operator++(i32) -> MutJumpIterReqMock&;
        auto operator--(i32) -> MutJumpIterReqMock&;

        auto operator+=(usize steps) -> MutJumpIterReqMock&;
        auto operator-=(usize steps) -> MutJumpIterReqMock&;

        auto operator+(usize steps) const -> MutJumpIterReqMock;
        auto operator-(usize steps) const -> MutJumpIterReqMock;

        auto operator-(const MutJumpIterReqMock& that) const -> isize;
    };

    ATOM_SATISFIES_MUT_JUMP_ITER_PAIR(MutJumpIterReqMock<i32>, IterEndReqMock);

    /// --------------------------------------------------------------------------------------------
    /// {RArrIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class ArrIterReqMock : public ArrIterTag
    {
    public:
        using TElem = T;

    public:
        auto operator*() -> T&;
        auto operator*() const -> const T&;

        auto operator==(ArrIterReqMock end) const -> bool;
        auto operator!=(ArrIterReqMock end) const -> bool;

        auto operator++(i32) -> ArrIterReqMock&;
        auto operator--(i32) -> ArrIterReqMock&;

        auto operator+=(usize steps) -> ArrIterReqMock&;
        auto operator-=(usize steps) -> ArrIterReqMock&;

        auto operator+(usize steps) const -> ArrIterReqMock;
        auto operator-(usize steps) const -> ArrIterReqMock;

        auto operator-(const ArrIterReqMock& that) const -> isize;
    };

    ATOM_SATISFIES_ARR_ITER_PAIR(ArrIterReqMock<i32>, ArrIterReqMock<i32>);

    /// --------------------------------------------------------------------------------------------
    /// {RMutArrIter} mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class MutArrIterReqMock : public ArrIterTag
    {
    public:
        using TElem = T;

    public:
        auto operator*() -> T&;
        auto operator*() const -> const T&;

        auto operator==(MutArrIterReqMock end) const -> bool;
        auto operator!=(MutArrIterReqMock end) const -> bool;

        auto operator++(i32) -> MutArrIterReqMock&;
        auto operator--(i32) -> MutArrIterReqMock&;

        auto operator+=(usize steps) -> MutArrIterReqMock&;
        auto operator-=(usize steps) -> MutArrIterReqMock&;

        auto operator+(usize steps) const -> MutArrIterReqMock;
        auto operator-(usize steps) const -> MutArrIterReqMock;

        auto operator-(const MutArrIterReqMock& that) const -> isize;
    };

    ATOM_SATISFIES_MUT_ARR_ITER_PAIR(MutArrIterReqMock<i32>, MutArrIterReqMock<i32>);
}
