#pragma once
#include "Atom/Core.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// Common requirements for every iterator type.
    /// --------------------------------------------------------------------------------------------
    template <typename TIter, typename TIterEnd, typename T>
    concept _RIterOf = requires(const TIter cit, const TIterEnd cend)
    {
        // requires typename TIter::TElem;

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
    concept _RMutIterOf = requires(TIter it)
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
    concept RIterOf = requires(TIter it)
    {
        requires _RIterOf<TIter, TIterEnd, T>;

        { it++ } -> RConvertibleTo<TIter&>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TIter} is {MutIter} of type {T} with end {TIterEnd}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIter, typename TIterEnd, typename T>
    concept RMutIterOf = requires(TIter it)
    {
        requires RIterOf<TIter, TIterEnd, T>;
        requires _RMutIterOf<TIter, TIterEnd, T>;
    };

    struct MultiPassIterTag { };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TIter} is {FwdIter} of type {T} with end {TIterEnd}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIter, typename TIterEnd, typename T>
    concept RFwdIterOf = requires
    {
        requires _RIterTag<TIter, MultiPassIterTag>;
        requires RIterOf<TIter, TIterEnd, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TIter} is {MutFwdIter} of type {T} with end {TIterEnd}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIter, typename TIterEnd, typename T>
    concept RMutFwdIterOf = requires
    {
        requires RFwdIterOf<TIter, TIterEnd, T>;
        requires _RMutIterOf<TIter, TIterEnd, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TIter} is {BidiIter} of type {T} with end {TIterEnd}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIter, typename TIterEnd, typename T>
    concept RBidiIterOf = requires(TIter it)
    {
        requires RFwdIterOf<TIter, TIterEnd, T>;

        { it-- } -> RConvertibleTo<TIter&>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TIter} is {MutBidiIter} of type {T} with end {TIterEnd}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIter, typename TIterEnd, typename T>
    concept RMutBidiIterOf = requires
    {
        requires RBidiIterOf<TIter, TIterEnd, T>;
        requires _RMutIterOf<TIter, TIterEnd, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TIter} is {JumpIter} of type {T} with end {TIterEnd}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIter, typename TIterEnd, typename T>
    concept RJumpIterOf = requires(TIter it, const TIter cit, usize steps)
    {
        requires RBidiIterOf<TIter, TIterEnd, T>;

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
    concept RMutJumpIterOf = requires
    {
        requires RJumpIterOf<TIter, TIterEnd, T>;
        requires _RMutIterOf<TIter, TIterEnd, T>;
    };

    struct ArrIterTag : MultiPassIterTag { };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TIter} is {ArrIter} of type {T} with end {TIterEnd}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIter, typename TIterEnd, typename T>
    concept RArrIterOf = requires
    {
        requires _RIterTag<TIter, ArrIterTag>;
        requires RJumpIterOf<TIter, TIterEnd, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TIter} is {MutArrIter} of type {T} with end {TIterEnd}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIter, typename TIterEnd, typename T>
    concept RMutArrIterOf = requires
    {
        requires _RIterTag<TIter, ArrIterTag>;
        requires RMutJumpIterOf<TIter, TIterEnd, T>;
    };
}
