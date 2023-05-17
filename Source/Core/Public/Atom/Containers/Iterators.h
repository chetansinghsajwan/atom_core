#pragma once
#include "Atom/Core.h"

namespace Atom::Private
{
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter, typename T>
    concept RIterBase = requires(TIter it)
    {
        { it.Get() } -> RConvertibleTo<T&>;

        // TODO: Fix this requirement.
        // { for (T& el : it) { } }
    };
}

namespace Atom
{
    /// Ensures {TIter} is {FwdIter} of type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIter, typename T>
    concept RFwdIter = requires(TIter it, const TIter cit)
    {
        requires Private::RIterBase<TIter, T>;

        { it.Next() }     -> RConvertibleTo<bool>;
        { cit.HasNext() } -> RConvertibleTo<bool>;
    };

    /// Ensures {TIter} is {BwdIter} of type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIter, typename T>
    concept RBwdIter = requires(TIter it, const TIter cit)
    {
        requires Private::RIterBase<TIter, T>;

        { it.Prev() }     -> RConvertibleTo<bool>;
        { cit.HasPrev() } -> RConvertibleTo<bool>;
    };

    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter, typename T>
    concept RFwdJumpIter = requires(TIter it, usize steps)
    {
        requires RFwdIter<TIter, T>;

        { it.Next(steps) } -> RConvertibleTo<bool>;
        { it.NextRange() } -> RConvertibleTo<usize>;
    };

    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter, typename T>
    concept RBwdJumpIter = requires(TIter it, usize steps)
    {
        requires RBwdIter<TIter, T>;

        { it.Prev(steps) } -> RConvertibleTo<bool>;
        { it.PrevRange() } -> RConvertibleTo<usize>;
    };

    /// Ensures {TIter} is {TwoWayIter} of type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIter, typename T>
    concept RTwoWayIter = requires
    {
        requires RFwdIter<TIter, T>;
        requires RBwdIter<TIter, T>;
    };

    /// Ensures {TIter} is {TwoWayIter} of type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIter, typename T>
    concept RTwoWayJumpIter = requires
    {
        requires RTwoWayIter<TIter, T>;

        requires RFwdJumpIter<TIter, T>;
        requires RBwdJumpIter<TIter, T>;
    };

    /// Ensures {TIter} is {ArrayIt} of type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIter, typename T>
    concept RArrayIter = requires(TIter it, const TIter cit)
    {
        requires RTwoWayJumpIter<TIter, T>;

        { it.Data() } -> RConvertibleTo<T*>;
    };

    /// Ensures {TIter} is {MultiPassIter} of type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIter, typename T>
    concept RMultiPassIter = requires
    {
        requires RCopyable<TIter>;
        requires RMoveable<TIter>;

        requires RFwdIter<TIter, T> || RBwdIter<TIter, T>;
    };
}
