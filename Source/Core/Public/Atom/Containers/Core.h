#pragma once
#include "Atom/Core.h"
#include "Atom/TTI.h"

namespace Atom
{
//// -----------------------------------------------------------------------------------------------
//// Concepts
//// -----------------------------------------------------------------------------------------------

    template <typename FromType, typename ToType>
    concept ConvertibleRequirements = TTI::ConvertibleTo<FromType, ToType>;

//// -----------------------------------------------------------------------------------------------
//// Iterator Requirements
//// -----------------------------------------------------------------------------------------------

    template <typename IteratorType, typename ElementType>
    concept ForwardIteratorRequirements = requires (IteratorType it)
    {
        { IteratorType::ElementType } -> TTI::ConvertibleTo<ElementType&>;

        { it.operator *  () }  -> TTI::ConvertibleTo<ElementType&>;
        { it.operator -> () }  -> TTI::ConvertibleTo<ElementType*>;
        { it.operator ++ () }  -> TTI::SameAs<IteratorType&>;
        { it.operator ++ (0) } -> TTI::SameAs<IteratorType>;
    };

    template <typename IteratorType, typename ElementType>
    concept ForwardIterator = ForwardIteratorRequirements<IteratorType, ElementType>;

    template <typename IteratorType, typename ElementType>
    concept ConstIterator = ForwardIteratorRequirements<IteratorType, ElementType>;

    template <typename IteratorType, typename ElementType>
    concept Iterator = ForwardIteratorRequirements<IteratorType, ElementType>;

    template <typename IteratorType, typename ElementType>
    concept BidirectionalIteratorRequirements = requires(IteratorType it)
    {
        requires ForwardIteratorRequirements<IteratorType, ElementType>;

        { it.operator -- () } -> TTI::SameAs<IteratorType&>;
        { it.operator -- (0) } -> TTI::SameAs<IteratorType>;
    };

    template <typename IteratorType, typename ElementType>
    concept RandomAccessIteratorRequirements = requires
    {
        requires BidirectionalIteratorRequirements<IteratorType, ElementType>;
    };

//// -----------------------------------------------------------------------------------------------
//// Iterable Requirements
//// -----------------------------------------------------------------------------------------------

    template <typename ConstIterableType, typename ElementType>
    concept ConstIterableRequirements = requires(ConstIterableType it)
    {
        typename ConstIterableType::IteratorType;

        it.cbegin();
        it.cend();

        it.begin();
        it.end();
    };

    template <typename IterableType, typename ElementType>
    concept IterableRequirements = requires(IterableType it)
    {
        requires ConstIterableRequirements<IterableType, ElementType>;

        it.begin();
        it.end();
    };

    template <typename ConstIterableType, typename ElementType>
    concept ConstIterable = ConstIterableRequirements<ConstIterableType, ElementType>;

    template <typename IterableType, typename ElementType>
    concept Iterable = IterableRequirements<IterableType, ElementType>;
}