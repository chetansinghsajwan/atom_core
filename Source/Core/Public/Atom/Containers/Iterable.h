#pragma once
#include "Atom/Containers/Iterator.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// ConstIterable: Represents a type over which you can iterate without changing the elements.
    /// --------------------------------------------------------------------------------------------

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TConstIterable} is {ConstIterable} for element type {TElement}.
    /// --------------------------------------------------------------------------------------------
    template <typename TConstIterable, typename TElement>
    concept RConstIterable = requires(const TConstIterable iterable)
    {
        requires RConstIterator<typename TConstIterable::TConstIterator, TElement>;

        { iterable.ConstBegin() } -> RSameAs<typename TConstIterable::TConstIterator>;
        { iterable.ConstEnd() }   -> RSameAs<typename TConstIterable::TConstIterator>;

        { iterable.Begin() }      -> RSameAs<typename TConstIterable::TConstIterator>;
        { iterable.End() }        -> RSameAs<typename TConstIterable::TConstIterator>;

        { iterable.cbegin() }     -> RSameAs<typename TConstIterable::TConstIterator>;
        { iterable.cend() }       -> RSameAs<typename TConstIterable::TConstIterator>;

        { iterable.begin() }      -> RSameAs<typename TConstIterable::TConstIterator>;
        { iterable.end() }        -> RSameAs<typename TConstIterable::TConstIterator>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Iterable: Represents a type over which you can iterate and change elements.
    /// --------------------------------------------------------------------------------------------

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TIterable} is {Iterable}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIterable, typename TElement>
    concept RIterable = requires(TIterable iterable)
    {
        requires RConstIterable<TIterable, TElement>;

        requires RIterator<typename TIterable::TIterator, TElement>;

        { iterable.Begin() } -> RSameAs<typename TIterable::TIterator>;
        { iterable.End() }   -> RSameAs<typename TIterable::TIterator>;

        { iterable.begin() } -> RSameAs<typename TIterable::TIterator>;
        { iterable.end() }   -> RSameAs<typename TIterable::TIterator>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Type to check if {RConstIterable} is accepted during defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename TElement>
    struct ConstIterableTestImpl
    {
        using TConstIterator = ConstIteratorTestImpl<TElement>;

        TConstIterator ConstBegin() const noexcept { return {}; };
        TConstIterator ConstEnd() const noexcept { return {}; };

        TConstIterator Begin() const noexcept { return {}; };
        TConstIterator End() const noexcept { return {}; };

        TConstIterator cbegin() const noexcept { return {}; };
        TConstIterator cend() const noexcept { return {}; };

        TConstIterator begin() const noexcept { return {}; };
        TConstIterator end() const noexcept { return {}; };
    };

    static_assert(RConstIterable<ConstIterableTestImpl<int>, int>,
        "ConstIterableTestImpl does not meet RConstIterable requirements.");

    /// --------------------------------------------------------------------------------------------
    /// Type to check if {RIterable} is accepted during defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename TElement>
    struct IterableTestImpl: public ConstIterableTestImpl<TElement>
    {
        using TConstIterator = ConstIteratorTestImpl<TElement>;
        using TIterator = IteratorTestImpl<TElement>;

        using ConstIterableTestImpl<TElement>::Begin;
        using ConstIterableTestImpl<TElement>::begin;

        using ConstIterableTestImpl<TElement>::End;
        using ConstIterableTestImpl<TElement>::end;

        constexpr TIterator Begin() noexcept { return {}; };
        constexpr TIterator End() noexcept { return {}; };

        constexpr TIterator begin() noexcept { return {}; };
        constexpr TIterator end() noexcept { return {}; };
    };

    static_assert(RIterable<IterableTestImpl<int>, int>,
        "IterableTestImpl does not meet RIterable requirements.");
}