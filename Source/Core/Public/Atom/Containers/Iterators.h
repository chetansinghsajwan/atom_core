#pragma once
#include "Atom/Core.h"

namespace Atom::Private
{
    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIt, typename T>
    concept RItBase = requires(TIt it)
    {
        { it.Get() } -> RConvertibleTo<const T&>;

        // TODO: Fix this requirement.
        // { for (T& el : it) { } }
    };
}

namespace Atom
{
    /// Ensures {TIt} is {FwdIt} of type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIt, typename T>
    concept RFwdIt = requires(TIt it, const TIt cit)
    {
        requires Private::RItBase<TIt, T>;

        { it.Next() }     -> RConvertibleTo<bool>;
        { cit.HasNext() } -> RConvertibleTo<bool>;
    };

    /// Ensures {TIt} is {BwdIt} of type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIt, typename T>
    concept RBwdIt = requires(TIt it, const TIt cit)
    {
        requires Private::RItBase<TIt, T>;

        { it.Prev() }     -> RConvertibleTo<bool>;
        { cit.HasPrev() } -> RConvertibleTo<bool>;
    };

    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIt, typename T>
    concept RFwdJumpIt = requires(TIt it, usize steps)
    {
        requires RFwdIt<TIt, T>;

        { it.Next(steps) } -> RConvertibleTo<bool>;
        { it.NextRange() } -> RConvertibleTo<usize>;
    };

    /// 
    /// --------------------------------------------------------------------------------------------
    template <typename TIt, typename T>
    concept RBwdJumpIt = requires(TIt it, usize steps)
    {
        requires RBwdIt<TIt, T>;

        { it.Prev(steps) } -> RConvertibleTo<bool>;
        { it.PrevRange() } -> RConvertibleTo<usize>;
    };

    /// Ensures {TIt} is {TwoWayIt} of type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIt, typename T>
    concept RTwoWayIt = requires
    {
        requires RFwdIt<TIt, T>;
        requires RBwdIt<TIt, T>;
    };

    /// Ensures {TIt} is {TwoWayIt} of type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIt, typename T>
    concept RTwoWayJumpIt = requires
    {
        requires RTwoWayIt<TIt, T>;

        requires RFwdJumpIt<TIt, T>;
        requires RBwdJumpIt<TIt, T>;
    };

    /// Ensures {TIt} is {ArrayIt} of type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIt, typename T>
    concept RArrayIt = requires(TIt it, const TIt cit)
    {
        requires RTwoWayJumpIt<TIt, T>;

        { it.Data() } -> RConvertibleTo<T*>;
    };

    /// Ensures {TIt} is {MultiPassIt} of type {T}.
    /// --------------------------------------------------------------------------------------------
    template <typename TIt, typename T>
    concept RMultiPassIt = requires
    {
        requires RCopyable<TIt>;
        requires RMoveable<TIt>;

        requires RFwdIt<TIt, T> || RBwdIt<TIt, T>;
    };
}

namespace Atom::Internal
{
    /// Type to test if a type implementing {RFwdIt} is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct FwdItMock
    {
        T& Get();

        bool Next();
        bool HasNext() const;
    };

    static_assert(RFwdIt<FwdItMock<int>, int>,
        "FwdItMock does not meet RFwdIt requirements.");

    /// Type to test if a type implementing {RBwdIt} is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct BwdItMock
    {
        T& Get();

        bool Prev();
        bool HasPrev() const;
    };

    static_assert(RBwdIt<BwdItMock<int>, int>,
        "BwdItMock does not meet RBwdIt requirements.");

    /// Type to test if a type implementing {RFwdJumpIt} is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct FwdJumpItMock: FwdItMock<T>
    {
        using FwdItMock<T>::Next;

        bool Next(usize steps);
        usize NextRange() const;
    };

    static_assert(RFwdJumpIt<FwdJumpItMock<int>, int>,
        "FwdJumpItMock does not meet RFwdJumpIt requirements.");

    /// Type to test if a type implementing {RBwdJumpIt} is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct BwdJumpItMock: BwdItMock<T>
    {
        using BwdItMock<T>::Prev;

        bool Prev(usize steps);
        usize PrevRange() const;
    };

    static_assert(RBwdJumpIt<BwdJumpItMock<int>, int>,
        "BwdJumpItMock does not meet RBwdJumpIt requirements.");

    /// Type to test if a type implementing {RTwoWayIt} is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct TwoWayItMock: FwdItMock<T>, BwdItMock<T>
    {
        T& Get();
    };

    static_assert(RTwoWayIt<TwoWayItMock<int>, int>,
        "TwoWayItMock does not meet RTwoWayIt requirements.");

    /// Type to test if a type implementing {RTwoWayJumpIt} is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct TwoWayJumpItMock: FwdJumpItMock<T>, BwdJumpItMock<T>
    {
        T& Get();
    };

    static_assert(RTwoWayJumpIt<TwoWayJumpItMock<int>, int>,
        "TwoWayJumpItMock does not meet RTwoWayJumpIt requirements.");

    /// Type to test if a type implementing {RArrayIt} is accepted when defining concepts.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    struct ArrayItMock: TwoWayJumpItMock<T>
    {
        T* Data();
    };

    static_assert(RArrayIt<ArrayItMock<int>, int>,
        "ArrayItMock does not meet RArrayIt requirements.");
}

namespace Atom
{
    template <typename TOutputWriter, typename T>
    concept ROutputWriter = requires(TOutputWriter out)
    {
        { out += declval(const T&) };
        { out += declval(T&&) };

        { out += Internal::FwdItMock<T>() };
    };
}

namespace Atom::Internal
{
    template <typename T>
    struct OutputWriterMock
    {
        void operator += (const T& in);
        void operator += (T&& in);

        void operator += (RFwdIt<T> auto in);
    };
}
