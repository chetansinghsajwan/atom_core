#pragma once
#include "Atom/Core.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// {OneWayIterator} is {ForwardIterator} as in {std}.
    /// {TwoWayIterator} is {BidirectionalIterator} as in {std}.
    /// {DirectIterator} is {RandomAccessIterator} as in {std}.
    /// --------------------------------------------------------------------------------------------

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TIterator} is {OneWayIterator} for type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIterator, typename T>
    concept ROneWayIterator = requires(TIterator iterator, const TIterator constIterator)
    {
        requires RCopyConstructible<TIterator>;
        requires RMoveConstructible<TIterator>;
        requires RCopyAssignable<TIterator>;
        requires RMoveAssignable<TIterator>;

        { iterator.Get() } -> RConvertibleTo<const T&>;
        { iterator.Next() } -> RSameAs<bool>;
        { constIterator.HasNext() } -> RSameAs<bool>;
    };

    template <typename TInputIterator, typename T>
    concept RInputIterator = ROneWayIterator<TInputIterator, T>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TIterator} is {TwoWayIterator} for type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIterator, typename T>
    concept RTwoWayIterator = requires(TIterator iterator, const TIterator constIterator)
    {
        requires ROneWayIterator<TIterator, T>;

        { iterator.Prev() } -> RSameAs<bool>;
        { constIterator.HasPrev() } -> RSameAs<bool>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TIterator} is {DirectIterator} for type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIterator, typename T>
    concept RDirectIterator = requires(TIterator iterator, const TIterator constIterator)
    {
        requires RTwoWayIterator<TIterator, T>;

        { iterator.Next((usize)0) } -> RSameAs<bool>;
        { iterator.Prev((usize)0) } -> RSameAs<bool>;

        { constIterator.Range() } -> RConvertibleTo<usize>;
    };
}

namespace Atom::Internal
{
    /// --------------------------------------------------------------------------------------------
    /// Type to test if a type implementing {ROneWayIterator} is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct OneWayIteratorMock
    {
        T& Get();

        bool Next();
        bool HasNext() const;
    };

    static_assert(ROneWayIterator<OneWayIteratorMock<int>, int>,
        "OneWayIteratorMock does not meet ROneWayIterator requirements.");

    template <typename T>
    using InputIteratorMock = OneWayIteratorMock<T>;

    /// --------------------------------------------------------------------------------------------
    /// Type to test if a type implementing {RTwoWayIterator} is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct TwoWayIteratorMock: OneWayIteratorMock<T>
    {
        bool Prev();
        bool HasPrev() const;
    };

    static_assert(RTwoWayIterator<TwoWayIteratorMock<int>, int>,
        "TwoWayIteratorMock does not meet RTwoWayIterator requirements.");

    /// --------------------------------------------------------------------------------------------
    /// Type to test if a type implementing {RDirectIterator} is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct DirectIteratorMock: TwoWayIteratorMock<T>
    {
        using OneWayIteratorMock<T>::Next;
        bool Next(usize steps);

        using TwoWayIteratorMock<T>::Prev;
        bool Prev(usize steps);

        usize Range() const;
    };

    static_assert(RDirectIterator<DirectIteratorMock<int>, int>,
        "DirectIteratorMock does not meet RDirectIterator requirements.");
}

namespace Atom
{
    template <typename TOutputWriter, typename T>
    concept ROutputWriter = requires(TOutputWriter out)
    {
        { out += declval(const T&) };
        { out += declval(T&&) };

        { out += Internal::InputIteratorMock<T>() };
    };
}

namespace Atom::Internal
{
    template <typename T>
    struct OutputWriterMock
    {
        void operator += (const T& in);
        void operator += (T&& in);

        void operator += (RInputIterator<T> auto in);
    };
}