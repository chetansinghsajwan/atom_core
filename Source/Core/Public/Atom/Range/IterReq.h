#pragma once
#include "Atom/Core.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// Common requirements for every iterator type.
    /// --------------------------------------------------------------------------------------------
    template <typename TIter, typename TIterEnd, typename T>
    concept _RIter = requires(const TIter cit, const TIterEnd cend)
    {
        requires RCopyable<TIter>;
        requires RMoveable<TIter>;

        requires RCopyable<TIterEnd>;
        requires RMoveable<TIterEnd>;

        { *cit } -> RConvertibleTo<const T&>;
        { cit == cend } -> RConvertibleTo<bool>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TIter} is {MutIter} of type {T} with end {TIterEnd}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIter, typename TIterEnd, typename T>
    concept _RMutIter = requires(TIter it)
    {
        { *it } -> RConvertibleTo<T&>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TIter} is {MutIter} of type {T} with end {TIterEnd}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIter, typename TTag>
    concept _RIterTag = requires
    {
        requires RDerivedFrom<TIter, TTag>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TIter} is {Iter} of type {T} with end {TIterEnd}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIter, typename TIterEnd, typename T>
    concept RIter = requires(TIter it)
    {
        requires _RIter<TIter, TIterEnd, T>;

        { it++ } -> RConvertibleTo<TIter&>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TIter} is {MutIter} of type {T} with end {TIterEnd}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIter, typename TIterEnd, typename T>
    concept RMutIter = requires(TIter it)
    {
        requires RIter<TIter, TIterEnd, T>;
        requires _RMutIter<TIter, TIterEnd, T>;
    };

    struct MultiPassIterTag { };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TIter} is {FwdIter} of type {T} with end {TIterEnd}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIter, typename TIterEnd, typename T>
    concept RFwdIter = requires
    {
        requires _RIterTag<TIter, MultiPassIterTag>;
        requires RIter<TIter, TIterEnd, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TIter} is {MutFwdIter} of type {T} with end {TIterEnd}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIter, typename TIterEnd, typename T>
    concept RMutFwdIter = requires
    {
        requires RFwdIter<TIter, TIterEnd, T>;
        requires _RMutIter<TIter, TIterEnd, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TIter} is {BidiIter} of type {T} with end {TIterEnd}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIter, typename TIterEnd, typename T>
    concept RBidiIter = requires(TIter it)
    {
        requires RFwdIter<TIter, TIterEnd, T>;

        { it-- } -> RConvertibleTo<TIter&>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TIter} is {MutBidiIter} of type {T} with end {TIterEnd}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIter, typename TIterEnd, typename T>
    concept RMutBidiIter = requires
    {
        requires RBidiIter<TIter, TIterEnd, T>;
        requires _RMutIter<TIter, TIterEnd, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TIter} is {JumpIter} of type {T} with end {TIterEnd}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIter, typename TIterEnd, typename T>
    concept RJumpIter = requires(TIter it, const TIter cit, usize steps)
    {
        requires RBidiIter<TIter, TIterEnd, T>;

        { cit + steps } -> RConvertibleTo<TIter>;
        { cit - steps } -> RConvertibleTo<TIter>;

        { it += steps } -> RConvertibleTo<TIter&>;
        { it -= steps } -> RConvertibleTo<TIter&>;

        { cit - cit } -> RConvertibleTo<isize>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TIter} is {MutJumpIter} of type {T} with end {TIterEnd}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIter, typename TIterEnd, typename T>
    concept RMutJumpIter = requires
    {
        requires RJumpIter<TIter, TIterEnd, T>;
        requires _RMutIter<TIter, TIterEnd, T>;
    };

    struct ArrIterTag : MultiPassIterTag { };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TIter} is {ArrIter} of type {T} with end {TIterEnd}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIter, typename TIterEnd, typename T>
    concept RArrIter = requires
    {
        requires _RIterTag<TIter, ArrIterTag>;
        requires RJumpIter<TIter, TIterEnd, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TIter} is {MutArrIter} of type {T} with end {TIterEnd}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIter, typename TIterEnd, typename T>
    concept RMutArrIter = requires
    {
        requires _RIterTag<TIter, ArrIterTag>;
        requires RMutJumpIter<TIter, TIterEnd, T>;
    };
}
