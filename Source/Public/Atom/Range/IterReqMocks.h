#pragma once
#include "Atom/Core.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// Mock object used as `IterEnd`.
    /// --------------------------------------------------------------------------------------------
    class IterEndReqMock
    {};

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
        auto eq(IterEndReqMock end) const -> bool;
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
        auto eq(IterEndReqMock end) const -> bool;
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
        auto eq(IterEndReqMock end) const -> bool;
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
        auto eq(IterEndReqMock end) const -> bool;
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
        auto eq(IterEndReqMock end) const -> bool;
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
        auto eq(IterEndReqMock end) const -> bool;
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
        auto eq(IterEndReqMock end) const -> bool;
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
        auto eq(IterEndReqMock end) const -> bool;
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
        auto data() const -> MemPtr<T>;
        auto next(usize steps = 1) -> ArrayIterReqMock&;
        auto prev(usize steps = 1) -> ArrayIterReqMock&;
        auto eq(ArrayIterReqMock end) const -> bool;
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
        auto data() const -> MemPtr<T>;
        auto mutData() -> MutMemPtr<T>;
        auto next(usize steps = 1) -> MutArrayIterReqMock&;
        auto prev(usize steps = 1) -> MutArrayIterReqMock&;
        auto eq(MutArrayIterReqMock end) const -> bool;
        auto compare(const MutArrayIterReqMock& that) const -> isize;
    };
}
