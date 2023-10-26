#pragma once
#include "Atom/Core.h"

namespace Atom
{
    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////
    //// _Helpers
    ////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename TIter, typename T>
    concept _RIterOf = requires
    {
        requires RConvertibleTo<typename TIter::TElem, T>;
    };

    template <typename TIter>
    concept _RMutIter = requires(TIter it)
    {
        { it.mutValue() } -> RConvertibleTo<typename TIter::TElem&>;
    };

    template <typename TIter, typename T>
    concept _RMutIterOf = requires
    {
        requires RSameAs<typename TIter::TElem, T>;
        requires _RMutIter<TIter>;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////
    //// Common
    ////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename TIter, typename TIterEnd>
    concept RIterWithEnd = requires(TIter it, TIterEnd itEnd)
    {
        requires RCopyable<TIterEnd>;
        requires RMoveable<TIterEnd>;
        requires RDestructible<TIterEnd>;

        { it.equals(itEnd) } -> RSameAs<bool>;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////
    //// Iter
    ////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename TIter>
    concept RIter = requires(TIter it, const TIter cit)
    {
        typename TIter::TElem;

        requires RMoveable<TIter>;
        requires RDestructible<TIter>;

        { cit.value() } -> RConvertibleTo<const typename TIter::TElem&>;
        { it.next() } -> RConvertibleTo<TIter&>;
    };

    template <typename TIter>
    concept RMutIter = requires
    {
        requires RIter<TIter>;
        requires _RMutIter<TIter>;
    };

    template <typename TIter, typename T>
    concept RIterOf = requires
    {
        requires RIter<TIter>;
        requires _RIterOf<TIter, T>;
    };

    template <typename TIter, typename T>
    concept RMutIterOf = requires
    {
        requires RMutIter<TIter>;
        requires _RMutIterOf<TIter, T>;
    };

    template <typename TIter, typename TIterEnd>
    concept RIterPair = requires
    {
        requires RIter<TIter>;
        requires RIterWithEnd<TIter, TIterEnd>;
    };

    template <typename TIter, typename TIterEnd>
    concept RMutIterPair = requires
    {
        requires RMutIter<TIter>;
        requires RIterWithEnd<TIter, TIterEnd>;
    };

    template <typename TIter, typename TIterEnd, typename T>
    concept RIterPairOf = requires
    {
        requires RIterOf<TIter, T>;
        requires RIterWithEnd<TIter, TIterEnd>;
    };

    template <typename TIter, typename TIterEnd, typename T>
    concept RMutIterPairOf = requires
    {
        requires RMutIterOf<TIter, T>;
        requires RIterWithEnd<TIter, TIterEnd>;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////
    //// FwdIter
    ////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename TIter>
    concept RFwdIter = requires
    {
        requires RIter<TIter>;
        requires RCopyable<TIter>;
    };

    template <typename TIter>
    concept RMutFwdIter = requires
    {
        requires RFwdIter<TIter>;
        requires _RMutIter<TIter>;
    };

    template <typename TIter, typename T>
    concept RFwdIterOf = requires
    {
        requires RFwdIter<TIter>;
        requires _RIterOf<TIter, T>;
    };

    template <typename TIter, typename T>
    concept RMutFwdIterOf = requires
    {
        requires RMutFwdIter<TIter>;
        requires _RMutIterOf<TIter, T>;
    };

    template <typename TIter, typename TIterEnd>
    concept RFwdIterPair = requires
    {
        requires RFwdIter<TIter>;
        requires RIterWithEnd<TIter, TIterEnd>;
    };

    template <typename TIter, typename TIterEnd>
    concept RMutFwdIterPair = requires
    {
        requires RMutFwdIter<TIter>;
        requires RIterWithEnd<TIter, TIterEnd>;
    };

    template <typename TIter, typename TIterEnd, typename T>
    concept RFwdIterPairOf = requires
    {
        requires RFwdIterOf<TIter, T>;
        requires RIterWithEnd<TIter, TIterEnd>;
    };

    template <typename TIter, typename TIterEnd, typename T>
    concept RMutFwdIterPairOf = requires
    {
        requires RMutFwdIterOf<TIter, T>;
        requires RIterWithEnd<TIter, TIterEnd>;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////
    //// BidiIter
    ////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename TIter>
    concept RBidiIter = requires(TIter it)
    {
        requires RFwdIter<TIter>;

        { it.prev() } -> RConvertibleTo<TIter&>;
    };

    template <typename TIter>
    concept RMutBidiIter = requires
    {
        requires RBidiIter<TIter>;
        requires _RMutIter<TIter>;
    };

    template <typename TIter, typename T>
    concept RBidiIterOf = requires
    {
        requires RBidiIter<TIter>;
        requires _RIterOf<TIter, T>;
    };

    template <typename TIter, typename T>
    concept RMutBidiIterOf = requires
    {
        requires RMutBidiIter<TIter>;
        requires _RMutIterOf<TIter, T>;
    };

    template <typename TIter, typename TIterEnd>
    concept RBidiIterPair = requires
    {
        requires RBidiIter<TIter>;
        requires RIterWithEnd<TIter, TIterEnd>;
    };

    template <typename TIter, typename TIterEnd>
    concept RMutBidiIterPair = requires
    {
        requires RMutBidiIter<TIter>;
        requires RIterWithEnd<TIter, TIterEnd>;
    };

    template <typename TIter, typename TIterEnd, typename T>
    concept RBidiIterPairOf = requires
    {
        requires RBidiIterOf<TIter, T>;
        requires RIterWithEnd<TIter, TIterEnd>;
    };

    template <typename TIter, typename TIterEnd, typename T>
    concept RMutBidiIterPairOf = requires
    {
        requires RMutBidiIterOf<TIter, T>;
        requires RIterWithEnd<TIter, TIterEnd>;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////
    //// JumpIter
    ////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename TIter>
    concept RJumpIter = requires(TIter it, const TIter cit, usize steps)
    {
        requires RBidiIter<TIter>;

        { it.next(steps) } -> RConvertibleTo<TIter&>;
        { it.prev(steps) } -> RConvertibleTo<TIter&>;
        { cit.compare(cit) } -> RConvertibleTo<isize>;
    };

    template <typename TIter>
    concept RMutJumpIter = requires
    {
        requires RJumpIter<TIter>;
        requires _RMutIter<TIter>;
    };

    template <typename TIter, typename T>
    concept RJumpIterOf = requires
    {
        requires RJumpIter<TIter>;
        requires _RIterOf<TIter, T>;
    };

    template <typename TIter, typename T>
    concept RMutJumpIterOf = requires
    {
        requires RMutJumpIter<TIter>;
        requires _RMutIterOf<TIter, T>;
    };

    template <typename TIter, typename TIterEnd>
    concept RJumpIterPair = requires
    {
        requires RJumpIter<TIter>;
        requires RIterWithEnd<TIter, TIterEnd>;
    };

    template <typename TIter, typename TIterEnd>
    concept RMutJumpIterPair = requires
    {
        requires RMutJumpIter<TIter>;
        requires RIterWithEnd<TIter, TIterEnd>;
    };

    template <typename TIter, typename TIterEnd, typename T>
    concept RJumpIterPairOf = requires
    {
        requires RJumpIterOf<TIter, T>;
        requires RIterWithEnd<TIter, TIterEnd>;
    };

    template <typename TIter, typename TIterEnd, typename T>
    concept RMutJumpIterPairOf = requires
    {
        requires RMutJumpIterOf<TIter, T>;
        requires RIterWithEnd<TIter, TIterEnd>;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////
    //// ArrayIter
    ////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    template <typename TIter>
    concept RArrayIter = requires(const TIter cit)
    {
        requires RJumpIter<TIter>;

        { cit.data() } -> RConvertibleTo<const typename TIter::TElem*>;
    };

    template <typename TIter>
    concept RMutArrayIter = requires(TIter it)
    {
        requires RArrayIter<TIter>;
        requires _RMutIter<TIter>;

        { it.mutData() } -> RConvertibleTo<typename TIter::TElem*>;
    };

    template <typename TIter, typename T>
    concept RArrayIterOf = requires
    {
        requires RArrayIter<TIter>;
        requires _RIterOf<TIter, T>;
    };

    template <typename TIter, typename T>
    concept RMutArrayIterOf = requires
    {
        requires RMutArrayIter<TIter>;
        requires _RMutIterOf<TIter, T>;
    };

    template <typename TIter, typename TIterEnd>
    concept RArrayIterPair = requires
    {
        requires RArrayIter<TIter>;
        requires RArrayIter<TIterEnd>;
        requires RIterWithEnd<TIter, TIterEnd>;
    };

    template <typename TIter, typename TIterEnd>
    concept RMutArrayIterPair = requires
    {
        requires RMutArrayIter<TIter>;
        requires RMutArrayIter<TIterEnd>;
        requires RIterWithEnd<TIter, TIterEnd>;
    };

    template <typename TIter, typename TIterEnd, typename T>
    concept RArrayIterPairOf = requires
    {
        requires RArrayIterOf<TIter, T>;
        requires RArrayIterOf<TIterEnd, T>;
        requires RIterWithEnd<TIter, TIterEnd>;
    };

    template <typename TIter, typename TIterEnd, typename T>
    concept RMutArrayIterPairOf = requires
    {
        requires RMutArrayIterOf<TIter, T>;
        requires RMutArrayIterOf<TIterEnd, T>;
        requires RIterWithEnd<TIter, TIterEnd>;
    };
}
