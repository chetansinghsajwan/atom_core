#pragma once
#include "Atom/Core.h"

namespace Atom
{
//// -----------------------------------------------------------------------------------------------
#pragma region _Helpers

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname TIter>
    concept _RIter = requires(const TIter cit)
    {
        tname TIter::TElem;

        requires RCopyable<TIter>;
        requires RMoveable<TIter>;

        { *cit } -> RConvertibleTo<const tname TIter::TElem&>;
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname TIter, tname T>
    concept _RIterOf = requires
    {
        requires _RIter<TIter>;
        requires RConvertibleTo<tname TIter::TElem, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname TIter>
    concept _RMutIter = requires(TIter it)
    {
        requires _RIter<TIter>;

        { *it } -> RConvertibleTo<tname TIter::TElem&>;
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <tname TIter, tname T>
    concept _RMutIterOf = requires
    {
        requires _RMutIter<TIter>;
        requires RSameAs<tname TIter::TElem, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TIter} is {MutIter} of type {T} with end {TIterEnd}.
    /// --------------------------------------------------------------------------------------------
    template <tname TIter, tname TTag>
    concept _RIterTag = requires
    {
        requires RDerivedFrom<TIter, TTag>;
    };

    class MultiPassIterTag { };

    class ArrIterTag extends MultiPassIterTag { };

#pragma endregion
//// -----------------------------------------------------------------------------------------------

//// -----------------------------------------------------------------------------------------------
#pragma region Iter

    template <tname TIter, tname TIterEnd>
    concept RIterEnd = requires(TIter it, TIterEnd itEnd)
    {
        requires RCopyable<TIterEnd>;
        requires RMoveable<TIterEnd>;

        { it == itEnd };
    };

    template <tname TIter>
    concept RIter = requires(TIter it)
    {
        requires _RIter<TIter>;

        { it++ } -> RConvertibleTo<TIter&>;
    };

    template <tname TIter>
    concept RMutIter = requires
    {
        requires _RMutIter<TIter>;
        requires RIter<TIter>;
    };

    template <tname TIter, tname T>
    concept RIterOf = requires
    {
        requires _RIterOf<TIter, T>;
        requires RIter<TIter>;
    };

    template <tname TIter, tname T>
    concept RMutIterOf = requires
    {
        requires _RMutIterOf<TIter, T>;
        requires RMutIter<TIter>;
    };

    template <tname TIter, tname TIterEnd>
    concept RIterPair = requires
    {
        requires RIter<TIter>;
        requires RIterEnd<TIter, TIterEnd>;
    };

    template <tname TIter, tname TIterEnd>
    concept RMutIterPair = requires
    {
        requires RMutIter<TIter>;
        requires RIterEnd<TIter, TIterEnd>;
    };

    template <tname TIter, tname TIterEnd, tname T>
    concept RIterPairOf = requires
    {
        requires RIterOf<TIter, T>;
        requires RIterEnd<TIter, TIterEnd>;
    };

    template <tname TIter, tname TIterEnd, tname T>
    concept RMutIterPairOf = requires
    {
        requires RMutIterOf<TIter, T>;
        requires RIterEnd<TIter, TIterEnd>;
    };

#pragma endregion
//// -----------------------------------------------------------------------------------------------

//// -----------------------------------------------------------------------------------------------
#pragma region FwdIter

    template <tname TIter>
    concept RFwdIter = requires
    {
        requires _RIterTag<TIter, MultiPassIterTag>;
        requires RIter<TIter>;
    };

    template <tname TIter>
    concept RMutFwdIter = requires
    {
        requires _RMutIter<TIter>;
        requires RFwdIter<TIter>;
    };

    template <tname TIter, tname T>
    concept RFwdIterOf = requires
    {
        requires _RIterOf<TIter, T>;
        requires RFwdIter<TIter>;
    };

    template <tname TIter, tname T>
    concept RMutFwdIterOf = requires
    {
        requires _RMutIterOf<TIter, T>;
        requires RMutFwdIter<TIter>;
    };

    template <tname TIter, tname TIterEnd>
    concept RFwdIterPair = requires
    {
        requires RFwdIter<TIter>;
        requires RIterEnd<TIter, TIterEnd>;
    };

    template <tname TIter, tname TIterEnd>
    concept RMutFwdIterPair = requires
    {
        requires RMutFwdIter<TIter>;
        requires RIterEnd<TIter, TIterEnd>;
    };

    template <tname TIter, tname TIterEnd, tname T>
    concept RFwdIterPairOf = requires
    {
        requires RFwdIterOf<TIter, T>;
        requires RIterEnd<TIter, TIterEnd>;
    };

    template <tname TIter, tname TIterEnd, tname T>
    concept RMutFwdIterPairOf = requires
    {
        requires RMutFwdIterOf<TIter, T>;
        requires RIterEnd<TIter, TIterEnd>;
    };

#pragma endregion
//// -----------------------------------------------------------------------------------------------

//// -----------------------------------------------------------------------------------------------
#pragma region BidiIter

    template <tname TIter>
    concept RBidiIter = requires(TIter it)
    {
        requires RFwdIter<TIter>;

        { it-- } -> RConvertibleTo<TIter&>;
    };

    template <tname TIter>
    concept RMutBidiIter = requires
    {
        requires _RMutIter<TIter>;
        requires RBidiIter<TIter>;
    };

    template <tname TIter, tname T>
    concept RBidiIterOf = requires
    {
        requires _RIterOf<TIter, T>;
        requires RBidiIter<TIter>;
    };

    template <tname TIter, tname T>
    concept RMutBidiIterOf = requires
    {
        requires _RMutIterOf<TIter, T>;
        requires RMutBidiIter<TIter>;
    };

    template <tname TIter, tname TIterEnd>
    concept RBidiIterPair = requires
    {
        requires RBidiIter<TIter>;
        requires RIterEnd<TIter, TIterEnd>;
    };

    template <tname TIter, tname TIterEnd>
    concept RMutBidiIterPair = requires
    {
        requires RMutBidiIter<TIter>;
        requires RIterEnd<TIter, TIterEnd>;
    };

    template <tname TIter, tname TIterEnd, tname T>
    concept RBidiIterPairOf = requires
    {
        requires RBidiIterOf<TIter, T>;
        requires RIterEnd<TIter, TIterEnd>;
    };

    template <tname TIter, tname TIterEnd, tname T>
    concept RMutBidiIterPairOf = requires
    {
        requires RMutBidiIterOf<TIter, T>;
        requires RIterEnd<TIter, TIterEnd>;
    };

#pragma endregion
//// -----------------------------------------------------------------------------------------------

//// -----------------------------------------------------------------------------------------------
#pragma region JumpIter

    template <tname TIter>
    concept RJumpIter = requires(TIter it, const TIter cit, isize steps)
    {
        requires RBidiIter<TIter>;

        { cit + steps } -> RConvertibleTo<TIter>;
        { cit - steps } -> RConvertibleTo<TIter>;

        { it += steps } -> RConvertibleTo<TIter&>;
        { it -= steps } -> RConvertibleTo<TIter&>;

        { cit - cit } -> RConvertibleTo<isize>;
    };

    template <tname TIter>
    concept RMutJumpIter = requires
    {
        requires _RMutIter<TIter>;
        requires RJumpIter<TIter>;
    };

    template <tname TIter, tname T>
    concept RJumpIterOf = requires
    {
        requires _RIterOf<TIter, T>;
        requires RJumpIter<TIter>;
    };

    template <tname TIter, tname T>
    concept RMutJumpIterOf = requires
    {
        requires _RMutIterOf<TIter, T>;
        requires RMutJumpIter<TIter>;
    };

    template <tname TIter, tname TIterEnd>
    concept RJumpIterPair = requires
    {
        requires RJumpIter<TIter>;
        requires RIterEnd<TIter, TIterEnd>;
    };

    template <tname TIter, tname TIterEnd>
    concept RMutJumpIterPair = requires
    {
        requires RMutJumpIter<TIter>;
        requires RIterEnd<TIter, TIterEnd>;
    };

    template <tname TIter, tname TIterEnd, tname T>
    concept RJumpIterPairOf = requires
    {
        requires RJumpIterOf<TIter, T>;
        requires RIterEnd<TIter, TIterEnd>;
    };

    template <tname TIter, tname TIterEnd, tname T>
    concept RMutJumpIterPairOf = requires
    {
        requires RMutJumpIterOf<TIter, T>;
        requires RIterEnd<TIter, TIterEnd>;
    };

#pragma endregion
//// -----------------------------------------------------------------------------------------------

//// -----------------------------------------------------------------------------------------------
#pragma region ArrIter

    template <tname TIter>
    concept RArrIter = requires
    {
        requires _RIterTag<TIter, ArrIterTag>;
        requires RJumpIter<TIter>;
    };

    template <tname TIter>
    concept RMutArrIter = requires
    {
        requires _RMutIter<TIter>;
        requires RArrIter<TIter>;
    };

    template <tname TIter, tname T>
    concept RArrIterOf = requires
    {
        requires _RIterOf<TIter, T>;
        requires RArrIter<TIter>;
    };

    template <tname TIter, tname T>
    concept RMutArrIterOf = requires
    {
        requires _RMutIterOf<TIter, T>;
        requires RMutArrIter<TIter>;
    };

    template <tname TIter, tname TIterEnd>
    concept RArrIterPair = requires
    {
        requires RArrIter<TIter>;
        requires RArrIter<TIterEnd>;
        requires RIterEnd<TIter, TIterEnd>;
    };

    template <tname TIter, tname TIterEnd>
    concept RMutArrIterPair = requires
    {
        requires RMutArrIter<TIter>;
        requires RMutArrIter<TIterEnd>;
        requires RIterEnd<TIter, TIterEnd>;
    };

    template <tname TIter, tname TIterEnd, tname T>
    concept RArrIterPairOf = requires
    {
        requires RArrIterOf<TIter, T>;
        requires RArrIterOf<TIterEnd, T>;
        requires RIterEnd<TIter, TIterEnd>;
    };

    template <tname TIter, tname TIterEnd, tname T>
    concept RMutArrIterPairOf = requires
    {
        requires RMutArrIterOf<TIter, T>;
        requires RMutArrIterOf<TIterEnd, T>;
        requires RIterEnd<TIter, TIterEnd>;
    };

#pragma endregion
//// -----------------------------------------------------------------------------------------------
}
