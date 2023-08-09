#pragma once
#include "Atom/Core.h"

namespace Atom
{
//// -----------------------------------------------------------------------------------------------
#pragma region _Helpers

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter>
    concept _RIter = requires(const TIter cit)
    {
        typename TIter::TElem;

        requires RCopyable<TIter>;
        requires RMoveable<TIter>;

        { *cit } -> RConvertibleTo<const typename TIter::TElem&>;
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter, typename T>
    concept _RIterOf = requires
    {
        requires _RIter<TIter>;
        requires RConvertibleTo<typename TIter::TElem, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter>
    concept _RMutIter = requires(TIter it)
    {
        requires _RIter<TIter>;

        { *it } -> RConvertibleTo<typename TIter::TElem&>;
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIter, typename T>
    concept _RMutIterOf = requires
    {
        requires _RMutIter<TIter>;
        requires RSameAs<typename TIter::TElem, T>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TIter} is {MutIter} of type {T} with end {TIterEnd}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIter, typename TTag>
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

    template <typename TIter, typename TIterEnd>
    concept RIterEnd = requires(TIter it, TIterEnd itEnd)
    {
        requires RCopyable<TIterEnd>;
        requires RMoveable<TIterEnd>;

        { it == itEnd };
    };

    template <typename TIter>
    concept RIter = requires(TIter it)
    {
        requires _RIter<TIter>;

        { it++ } -> RConvertibleTo<TIter&>;
    };

    template <typename TIter>
    concept RMutIter = requires
    {
        requires _RMutIter<TIter>;
        requires RIter<TIter>;
    };

    template <typename TIter, typename T>
    concept RIterOf = requires
    {
        requires _RIterOf<TIter, T>;
        requires RIter<TIter>;
    };

    template <typename TIter, typename T>
    concept RMutIterOf = requires
    {
        requires _RMutIterOf<TIter, T>;
        requires RMutIter<TIter>;
    };

    template <typename TIter, typename TIterEnd>
    concept RIterPair = requires
    {
        requires RIter<TIter>;
        requires RIterEnd<TIter, TIterEnd>;
    };

    template <typename TIter, typename TIterEnd>
    concept RMutIterPair = requires
    {
        requires RMutIter<TIter>;
        requires RIterEnd<TIter, TIterEnd>;
    };

    template <typename TIter, typename TIterEnd, typename T>
    concept RIterPairOf = requires
    {
        requires RIterOf<TIter, T>;
        requires RIterEnd<TIter, TIterEnd>;
    };

    template <typename TIter, typename TIterEnd, typename T>
    concept RMutIterPairOf = requires
    {
        requires RMutIterOf<TIter, T>;
        requires RIterEnd<TIter, TIterEnd>;
    };

#pragma endregion
//// -----------------------------------------------------------------------------------------------

//// -----------------------------------------------------------------------------------------------
#pragma region FwdIter

    template <typename TIter>
    concept RFwdIter = requires
    {
        requires _RIterTag<TIter, MultiPassIterTag>;
        requires RIter<TIter>;
    };

    template <typename TIter>
    concept RMutFwdIter = requires
    {
        requires _RMutIter<TIter>;
        requires RFwdIter<TIter>;
    };

    template <typename TIter, typename T>
    concept RFwdIterOf = requires
    {
        requires _RIterOf<TIter, T>;
        requires RFwdIter<TIter>;
    };

    template <typename TIter, typename T>
    concept RMutFwdIterOf = requires
    {
        requires _RMutIterOf<TIter, T>;
        requires RMutFwdIter<TIter>;
    };

    template <typename TIter, typename TIterEnd>
    concept RFwdIterPair = requires
    {
        requires RFwdIter<TIter>;
        requires RIterEnd<TIter, TIterEnd>;
    };

    template <typename TIter, typename TIterEnd>
    concept RMutFwdIterPair = requires
    {
        requires RMutFwdIter<TIter>;
        requires RIterEnd<TIter, TIterEnd>;
    };

    template <typename TIter, typename TIterEnd, typename T>
    concept RFwdIterPairOf = requires
    {
        requires RFwdIterOf<TIter, T>;
        requires RIterEnd<TIter, TIterEnd>;
    };

    template <typename TIter, typename TIterEnd, typename T>
    concept RMutFwdIterPairOf = requires
    {
        requires RMutFwdIterOf<TIter, T>;
        requires RIterEnd<TIter, TIterEnd>;
    };

#pragma endregion
//// -----------------------------------------------------------------------------------------------

//// -----------------------------------------------------------------------------------------------
#pragma region BidiIter

    template <typename TIter>
    concept RBidiIter = requires(TIter it)
    {
        requires RFwdIter<TIter>;

        { it-- } -> RConvertibleTo<TIter&>;
    };

    template <typename TIter>
    concept RMutBidiIter = requires
    {
        requires _RMutIter<TIter>;
        requires RBidiIter<TIter>;
    };

    template <typename TIter, typename T>
    concept RBidiIterOf = requires
    {
        requires _RIterOf<TIter, T>;
        requires RBidiIter<TIter>;
    };

    template <typename TIter, typename T>
    concept RMutBidiIterOf = requires
    {
        requires _RMutIterOf<TIter, T>;
        requires RMutBidiIter<TIter>;
    };

    template <typename TIter, typename TIterEnd>
    concept RBidiIterPair = requires
    {
        requires RBidiIter<TIter>;
        requires RIterEnd<TIter, TIterEnd>;
    };

    template <typename TIter, typename TIterEnd>
    concept RMutBidiIterPair = requires
    {
        requires RMutBidiIter<TIter>;
        requires RIterEnd<TIter, TIterEnd>;
    };

    template <typename TIter, typename TIterEnd, typename T>
    concept RBidiIterPairOf = requires
    {
        requires RBidiIterOf<TIter, T>;
        requires RIterEnd<TIter, TIterEnd>;
    };

    template <typename TIter, typename TIterEnd, typename T>
    concept RMutBidiIterPairOf = requires
    {
        requires RMutBidiIterOf<TIter, T>;
        requires RIterEnd<TIter, TIterEnd>;
    };

#pragma endregion
//// -----------------------------------------------------------------------------------------------

//// -----------------------------------------------------------------------------------------------
#pragma region JumpIter

    template <typename TIter>
    concept RJumpIter = requires(TIter it, const TIter cit, isize steps)
    {
        requires RBidiIter<TIter>;

        { cit + steps } -> RConvertibleTo<TIter>;
        { cit - steps } -> RConvertibleTo<TIter>;

        { it += steps } -> RConvertibleTo<TIter&>;
        { it -= steps } -> RConvertibleTo<TIter&>;

        { cit - cit } -> RConvertibleTo<isize>;
    };

    template <typename TIter>
    concept RMutJumpIter = requires
    {
        requires _RMutIter<TIter>;
        requires RJumpIter<TIter>;
    };

    template <typename TIter, typename T>
    concept RJumpIterOf = requires
    {
        requires _RIterOf<TIter, T>;
        requires RJumpIter<TIter>;
    };

    template <typename TIter, typename T>
    concept RMutJumpIterOf = requires
    {
        requires _RMutIterOf<TIter, T>;
        requires RMutJumpIter<TIter>;
    };

    template <typename TIter, typename TIterEnd>
    concept RJumpIterPair = requires
    {
        requires RJumpIter<TIter>;
        requires RIterEnd<TIter, TIterEnd>;
    };

    template <typename TIter, typename TIterEnd>
    concept RMutJumpIterPair = requires
    {
        requires RMutJumpIter<TIter>;
        requires RIterEnd<TIter, TIterEnd>;
    };

    template <typename TIter, typename TIterEnd, typename T>
    concept RJumpIterPairOf = requires
    {
        requires RJumpIterOf<TIter, T>;
        requires RIterEnd<TIter, TIterEnd>;
    };

    template <typename TIter, typename TIterEnd, typename T>
    concept RMutJumpIterPairOf = requires
    {
        requires RMutJumpIterOf<TIter, T>;
        requires RIterEnd<TIter, TIterEnd>;
    };

#pragma endregion
//// -----------------------------------------------------------------------------------------------

//// -----------------------------------------------------------------------------------------------
#pragma region ArrIter

    template <typename TIter>
    concept RArrIter = requires
    {
        requires _RIterTag<TIter, ArrIterTag>;
        requires RJumpIter<TIter>;
    };

    template <typename TIter>
    concept RMutArrIter = requires
    {
        requires _RMutIter<TIter>;
        requires RArrIter<TIter>;
    };

    template <typename TIter, typename T>
    concept RArrIterOf = requires
    {
        requires _RIterOf<TIter, T>;
        requires RArrIter<TIter>;
    };

    template <typename TIter, typename T>
    concept RMutArrIterOf = requires
    {
        requires _RMutIterOf<TIter, T>;
        requires RMutArrIter<TIter>;
    };

    template <typename TIter, typename TIterEnd>
    concept RArrIterPair = requires
    {
        requires RArrIter<TIter>;
        requires RArrIter<TIterEnd>;
        requires RIterEnd<TIter, TIterEnd>;
    };

    template <typename TIter, typename TIterEnd>
    concept RMutArrIterPair = requires
    {
        requires RMutArrIter<TIter>;
        requires RMutArrIter<TIterEnd>;
        requires RIterEnd<TIter, TIterEnd>;
    };

    template <typename TIter, typename TIterEnd, typename T>
    concept RArrIterPairOf = requires
    {
        requires RArrIterOf<TIter, T>;
        requires RArrIterOf<TIterEnd, T>;
        requires RIterEnd<TIter, TIterEnd>;
    };

    template <typename TIter, typename TIterEnd, typename T>
    concept RMutArrIterPairOf = requires
    {
        requires RMutArrIterOf<TIter, T>;
        requires RMutArrIterOf<TIterEnd, T>;
        requires RIterEnd<TIter, TIterEnd>;
    };

#pragma endregion
//// -----------------------------------------------------------------------------------------------
}
