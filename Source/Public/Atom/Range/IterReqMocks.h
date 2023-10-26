#pragma once
#include "IterReqChecks.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// Mock object used as `IterEnd`.
    /// --------------------------------------------------------------------------------------------
    class IterEndReqMock {};

    /// --------------------------------------------------------------------------------------------
    /// `RIter` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class IterReqMock
    {
    public:
        using TElem = T;

    public:
        IterReqMock() = delete;
        IterReqMock(const IterReqMock& that) = delete;
        IterReqMock& operator=(const IterReqMock& that) = delete;

        IterReqMock(IterReqMock&& that) = default;
        IterReqMock& operator=(IterReqMock&& that) = default;
        ~IterReqMock() = default;

    public:
        auto value() const -> const T&;
        auto equals(IterEndReqMock end) const -> bool;
        auto next() -> IterReqMock&;
    };

    /// --------------------------------------------------------------------------------------------
    /// `RMutIter` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class MutIterReqMock
    {
    public:
        using TElem = T;

    public:
        MutIterReqMock() = delete;
        MutIterReqMock(const MutIterReqMock& that) = delete;
        MutIterReqMock& operator=(const MutIterReqMock& that) = delete;

        MutIterReqMock(MutIterReqMock&& that) = default;
        MutIterReqMock& operator=(MutIterReqMock&& that) = default;
        ~MutIterReqMock() = default;

    public:
        auto value() const -> const T&;
        auto mutValue() -> T&;
        auto equals(IterEndReqMock end) const -> bool;
        auto next() -> MutIterReqMock&;
    };

    /// --------------------------------------------------------------------------------------------
    /// `RFwdIter` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class FwdIterReqMock
    {
    public:
        using TElem = T;

    public:
        auto value() const -> const T&;
        auto mutValue() -> T&;
        auto next() -> FwdIterReqMock&;
        auto equals(IterEndReqMock end) const -> bool;
    };

    /// --------------------------------------------------------------------------------------------
    /// `RMutFwdIter` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class MutFwdIterReqMock
    {
    public:
        using TElem = T;

    public:
        auto value() const -> const T&;
        auto mutValue() -> T&;
        auto next() -> MutFwdIterReqMock&;
        auto equals(IterEndReqMock end) const -> bool;
    };

    /// --------------------------------------------------------------------------------------------
    /// `RBidiIter` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class BidiIterReqMock
    {
    public:
        using TElem = T;

    public:
        auto value() const -> const T&;
        auto next() -> BidiIterReqMock&;
        auto prev() -> BidiIterReqMock&;
        auto equals(IterEndReqMock end) const -> bool;
    };

    /// --------------------------------------------------------------------------------------------
    /// `RMutBidiIter` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class MutBidiIterReqMock
    {
    public:
        using TElem = T;

    public:
        auto value() const -> const T&;
        auto mutValue() -> T&;
        auto next() -> MutBidiIterReqMock&;
        auto prev() -> MutBidiIterReqMock&;
        auto equals(IterEndReqMock end) const -> bool;
    };

    /// --------------------------------------------------------------------------------------------
    /// `RJumpIter` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class JumpIterReqMock
    {
    public:
        using TElem = T;

    public:
        auto value() const -> const T&;
        auto next(usize steps = 1) -> JumpIterReqMock&;
        auto prev(usize steps = 1) -> JumpIterReqMock&;
        auto equals(IterEndReqMock end) const -> bool;
        auto compare(const JumpIterReqMock& that) const -> isize;
    };

    /// --------------------------------------------------------------------------------------------
    /// `RMutJumpIter` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class MutJumpIterReqMock
    {
    public:
        using TElem = T;

    public:
        auto value() const -> const T&;
        auto mutValue() -> T&;
        auto next(usize steps = 1) -> MutJumpIterReqMock&;
        auto prev(usize steps = 1) -> MutJumpIterReqMock&;
        auto equals(IterEndReqMock end) const -> bool;
        auto compare(const MutJumpIterReqMock& that) const -> isize;
    };

    /// --------------------------------------------------------------------------------------------
    /// `RArrayIter` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class ArrayIterReqMock
    {
    public:
        using TElem = T;

    public:
        auto value() const -> const T&;
        auto data() const -> const T*;
        auto next(usize steps = 1) -> ArrayIterReqMock&;
        auto prev(usize steps = 1) -> ArrayIterReqMock&;
        auto equals(ArrayIterReqMock end) const -> bool;
        auto compare(const ArrayIterReqMock& that) const -> isize;
    };

    /// --------------------------------------------------------------------------------------------
    /// `RMutArrayIter` mock object.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    class MutArrayIterReqMock
    {
    public:
        using TElem = T;

    public:
        auto value() const -> const T&;
        auto mutValue() -> T&;
        auto data() const -> const T*;
        auto mutData() -> T*;
        auto next(usize steps = 1) -> MutArrayIterReqMock&;
        auto prev(usize steps = 1) -> MutArrayIterReqMock&;
        auto equals(MutArrayIterReqMock end) const -> bool;
        auto compare(const MutArrayIterReqMock& that) const -> isize;
    };

////////////////////////////////////////////////////////////////////////////////////////////////////
////
//// # To Do: Move these checks into tests file.
////
////////////////////////////////////////////////////////////////////////////////////////////////////

    ATOM_SATISFIES_ITER_PAIR(IterReqMock<i32>, IterEndReqMock);
    ATOM_SATISFIES_MUT_ITER_PAIR(MutIterReqMock<i32>, IterEndReqMock);
    ATOM_SATISFIES_FWD_ITER_PAIR(FwdIterReqMock<i32>, IterEndReqMock);
    ATOM_SATISFIES_MUT_FWD_ITER_PAIR(MutFwdIterReqMock<i32>, IterEndReqMock);
    ATOM_SATISFIES_BIDI_ITER_PAIR(BidiIterReqMock<i32>, IterEndReqMock);
    ATOM_SATISFIES_MUT_BIDI_ITER_PAIR(MutBidiIterReqMock<i32>, IterEndReqMock);
    ATOM_SATISFIES_JUMP_ITER_PAIR(JumpIterReqMock<i32>, IterEndReqMock);
    ATOM_SATISFIES_MUT_JUMP_ITER_PAIR(MutJumpIterReqMock<i32>, IterEndReqMock);
    ATOM_SATISFIES_ARR_ITER_PAIR(ArrayIterReqMock<i32>, ArrayIterReqMock<i32>);
    ATOM_SATISFIES_MUT_ARR_ITER_PAIR(MutArrayIterReqMock<i32>, MutArrayIterReqMock<i32>);
}
