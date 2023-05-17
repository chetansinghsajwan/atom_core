#pragma once
#include "Atom/Core.h"

namespace Atom::Private
{
    /// Common requirements for every iterator type.
    /// --------------------------------------------------------------------------------------------
    template <typename TIter, typename TEnd, typename T>
    concept RIterBase = requires(TIter it, TEnd end)
    {
        requires RCopyable<TIter>;
        requires RMoveable<TIter>;

        requires RCopyable<TEnd>;
        requires RMoveable<TEnd>;

        { *it } -> RConvertibleTo<T&>;
        { it == end } -> RConvertibleTo<bool>;
    };
}

namespace Atom
{
    /// Ensures {TIter} is {FwdIter} of type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIter, typename TEnd, typename T>
    concept RFwdIter = requires(TIter it)
    {
        requires Private::RIterBase<TIter, TEnd, T>;

        { it++ } -> RConvertibleTo<TIter&>;
    };

    /// Ensures {TIter} is {BwdIter} of type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIter, typename TEnd, typename T>
    concept RBwdIter = requires(TIter it)
    {
        requires Private::RIterBase<TIter, TEnd, T>;

        { it-- } -> RConvertibleTo<TIter&>;
    };

    /// Ensures {TIter} is {FwdJumpIt} of type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIter, typename TEnd, typename T>
    concept RFwdJumpIter = requires(TIter it, usize steps)
    {
        requires RFwdIter<TIter, TEnd, T>;

        { it + steps } -> RConvertibleTo<TIter>;
        { it - it } -> RConvertibleTo<usize>;
    };

    /// Ensures {TIter} is {BwdJumpIt} of type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIter, typename TEnd, typename T>
    concept RBwdJumpIter = requires(TIter it, usize steps)
    {
        requires RBwdIter<TIter, TEnd, T>;

        { it - steps } -> RConvertibleTo<TIter>;
        { it - it } -> RConvertibleTo<usize>;
    };

    /// Ensures {TIter} is {TwoWayIter} of type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIter, typename TEnd, typename T>
    concept RTwoWayIter = requires
    {
        requires RFwdIter<TIter, TEnd, T>;
        requires RBwdIter<TIter, TEnd, T>;
    };

    /// Ensures {TIter} is {TwoWayIter} of type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIter, typename TEnd, typename T>
    concept RTwoWayJumpIter = requires
    {
        requires RTwoWayIter<TIter, TEnd, T>;

        requires RFwdJumpIter<TIter, TEnd, T>;
        requires RBwdJumpIter<TIter, TEnd, T>;
    };

    struct MultiPassIterTag { };

    /// Ensures {TIter} is {MultiPassIter} of type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIter, typename TEnd, typename T>
    concept RMultiPassIter = requires
    {
        requires RDerivedFrom<TIter, MultiPassIterTag>;
        requires RFwdIter<TIter, TEnd, T> || RBwdIter<TIter, TEnd, T>;
    };

    struct ArrayIterTag : MultiPassIterTag { };

    /// Ensures {TIter} is {ArrayIt} of type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIter, typename TEnd, typename T>
    concept RArrayIter = requires(TIter it)
    {
        requires RDerivedFrom<TIter, ArrayIterTag>;
        requires RTwoWayJumpIter<TIter, TEnd, T>;
        requires RMultiPassIter<TIter, TEnd, T>;
    };
}
