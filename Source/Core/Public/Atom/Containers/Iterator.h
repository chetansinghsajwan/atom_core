#pragma once
#include "Atom/Core.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// Ensures {TConstIterator} is {ConstIterator} for element type {TElement}.
    /// --------------------------------------------------------------------------------------------
    template <typename TConstIterator, typename TElement>
    concept RConstIterator = requires(const TConstIterator cit, TConstIterator it)
    {
        { cit.operator * () } -> RSameAs<const TElement&>;
        { cit.operator -> () } -> RSameAs<const TElement*>;

        { ++it } -> RSameAs<TConstIterator&>;
        { it++ } -> RSameAs<TConstIterator>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TIterator} is {Iterator} for element type {TElement}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIterator, typename TElement>
    concept RIterator = requires(TIterator it)
    {
        requires RConstIterator<TIterator, TElement>;

        { it.operator * () } -> RSameAs<TElement&>;
        { it.operator -> () } -> RSameAs<TElement*>;

        { ++it } -> RSameAs<TIterator&>;
        { it++ } -> RSameAs<TIterator>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Type to test if {RConstIterator} is accepted during defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename TElement>
    struct ConstIteratorTestImpl
    {
        const TElement& operator * () const noexcept
        {
            return (const TElement&)0;
        }

        const TElement* operator -> () const noexcept
        {
            return (const TElement*)0;
        }

        ConstIteratorTestImpl<TElement>& operator ++ () noexcept
        {
            return *this;
        }

        ConstIteratorTestImpl<TElement> operator ++ (int) noexcept
        {
            return *this;
        }
    };

    static_assert(RConstIterator<ConstIteratorTestImpl<int>, int>,
        "ConstIteratorTestImpl does not meet RConstIterator requirements.");

    /// --------------------------------------------------------------------------------------------
    /// Type to test if {RIterator} is accepted during defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename TElement>
    struct IteratorTestImpl: public ConstIteratorTestImpl<TElement>
    {
        using ConstIteratorTestImpl<TElement>::operator *;
        using ConstIteratorTestImpl<TElement>::operator ->;

        TElement& operator * () noexcept
        {
            return (TElement&)0;
        }

        TElement* operator -> () noexcept
        {
            return (TElement*)0;
        }

        IteratorTestImpl<TElement>& operator ++ () noexcept
        {
            return *this;
        };

        IteratorTestImpl<TElement> operator ++ (int) noexcept
        {
            return *this;
        };
    };

    static_assert(RIterator<IteratorTestImpl<int>, int>,
        "IteratorTestImpl does not meet RIterator requirements.");
}