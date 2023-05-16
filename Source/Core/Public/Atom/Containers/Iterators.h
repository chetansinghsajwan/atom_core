#pragma once
#include "Atom/Core.h"

namespace Atom::Private
{
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIt, typename T>
    concept RItBase = requires(TIt it)
    {
        { it.Get() } -> RConvertibleTo<T&>;

        // TODO: Fix this requirement.
        // { for (T& el : it) { } }
    };
}

namespace Atom
{
    /// Ensures {TIt} is {FwdIt} of type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIt, typename T>
    concept RFwdIt = requires(TIt it, const TIt cit)
    {
        requires Private::RItBase<TIt, T>;

        { it.Next() }     -> RConvertibleTo<bool>;
        { cit.HasNext() } -> RConvertibleTo<bool>;
    };

    /// Ensures {TIt} is {BwdIt} of type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIt, typename T>
    concept RBwdIt = requires(TIt it, const TIt cit)
    {
        requires Private::RItBase<TIt, T>;

        { it.Prev() }     -> RConvertibleTo<bool>;
        { cit.HasPrev() } -> RConvertibleTo<bool>;
    };

    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIt, typename T>
    concept RFwdJumpIt = requires(TIt it, usize steps)
    {
        requires RFwdIt<TIt, T>;

        { it.Next(steps) } -> RConvertibleTo<bool>;
        { it.NextRange() } -> RConvertibleTo<usize>;
    };

    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIt, typename T>
    concept RBwdJumpIt = requires(TIt it, usize steps)
    {
        requires RBwdIt<TIt, T>;

        { it.Prev(steps) } -> RConvertibleTo<bool>;
        { it.PrevRange() } -> RConvertibleTo<usize>;
    };

    /// Ensures {TIt} is {TwoWayIt} of type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIt, typename T>
    concept RTwoWayIt = requires
    {
        requires RFwdIt<TIt, T>;
        requires RBwdIt<TIt, T>;
    };

    /// Ensures {TIt} is {TwoWayIt} of type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIt, typename T>
    concept RTwoWayJumpIt = requires
    {
        requires RTwoWayIt<TIt, T>;

        requires RFwdJumpIt<TIt, T>;
        requires RBwdJumpIt<TIt, T>;
    };

    /// Ensures {TIt} is {ArrayIt} of type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIt, typename T>
    concept RArrayIt = requires(TIt it, const TIt cit)
    {
        requires RTwoWayJumpIt<TIt, T>;

        { it.Data() } -> RConvertibleTo<T*>;
    };

    /// Ensures {TIt} is {MultiPassIt} of type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIt, typename T>
    concept RMultiPassIt = requires
    {
        requires RCopyable<TIt>;
        requires RMoveable<TIt>;

        requires RFwdIt<TIt, T> || RBwdIt<TIt, T>;
    };
}
