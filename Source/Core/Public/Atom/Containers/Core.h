#pragma once
#include "Atom/Core.h"
#include "Atom/TTI.h"

namespace Atom
{
//// -----------------------------------------------------------------------------------------------
//// Iterator Requirements
//// -----------------------------------------------------------------------------------------------

    template <typename TIterator, typename TElement>
    concept RForwardIterator = requires (TIterator it)
    {
        { TIterator::TElement } -> TTI::RConvertible<TElement&>;

        { it.operator *  () }  -> TTI::RConvertible<TElement&>;
        { it.operator -> () }  -> TTI::RConvertible<TElement*>;
        { it.operator ++ () }  -> TTI::RSameAs<TIterator&>;
        { it.operator ++ (0) } -> TTI::RSameAs<TIterator>;
    };

    template <typename TIterator, typename TElement>
    concept RConstIterator = RForwardIterator<TIterator, TElement>;

    template <typename TIterator, typename TElement>
    concept RIterator = RForwardIterator<TIterator, TElement>;

    template <typename TIterator, typename TElement>
    concept RBidirectionalIterator = requires(TIterator it)
    {
        requires RForwardIterator<TIterator, TElement>;

        { it.operator -- () } -> TTI::RSameAs<TIterator&>;
        { it.operator -- (0) } -> TTI::RSameAs<TIterator>;
    };

    template <typename TIterator, typename TElement>
    concept RRandomAccessIterator = requires
    {
        requires RBidirectionalIterator<TIterator, TElement>;
    };

//// -----------------------------------------------------------------------------------------------
//// Iterable Requirements
//// -----------------------------------------------------------------------------------------------

    template <typename TConstIterable, typename TElement>
    concept RConstIterable = requires(TConstIterable it)
    {
        typename TConstIterable::TIterator;

        it.cbegin();
        it.cend();

        it.begin();
        it.end();
    };

    template <typename TIterable, typename TElement>
    concept RIterable = requires(TIterable it)
    {
        requires RConstIterable<TIterable, TElement>;

        it.begin();
        it.end();
    };
}