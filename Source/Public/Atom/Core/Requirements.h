#pragma once
#include "Atom/Core/LangExtensions.h"

#include <concepts>
#include <type_traits>

// clang-format off
namespace Atom
{
    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////
    //// Basic Concepts
    ////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    /// --------------------------------------------------------------------------------------------
    /// Ensures `T0` is same as `T1`.
    /// --------------------------------------------------------------------------------------------
    template <typename T0, typename T1>
    concept RSameAs = std::same_as<T0, T1>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures `T0` is `void`.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RIsVoid = RSameAs<T, void>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures unqualified type of `T0` is same as unqualified type of `T1`.
    /// --------------------------------------------------------------------------------------------
    template <typename T0, typename T1>
    concept RSameAsUnqualified = std::same_as<std::remove_cvref_t<T0>, std::remove_cvref_t<T1>>;

    /// --------------------------------------------------------------------------------------------
    /// Enusres `T` is const-qualified.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RConst = std::is_const_v<T>;

    /// --------------------------------------------------------------------------------------------
    /// Enusres `T` is volatile-qualified.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RVolatile = std::is_volatile_v<T>;

    /// --------------------------------------------------------------------------------------------
    /// Enusres `T` is ref.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RRef = std::is_reference_v<T>;

    /// --------------------------------------------------------------------------------------------
    /// Enusres `T` is l-value ref.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RLValueRef = std::is_lvalue_reference_v<T>;

    /// --------------------------------------------------------------------------------------------
    /// Enusres `T` is r-value ref.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RRValueRef = std::is_rvalue_reference_v<T>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures `TFrom` is `Convertible` to `TTo`.
    /// --------------------------------------------------------------------------------------------
    template <typename TFrom, typename TTo>
    concept RConvertibleTo = requires(TFrom from) { static_cast<TTo>(from); };

    /// --------------------------------------------------------------------------------------------
    /// Ensures `TDerived` is derived from `TBase`.
    /// --------------------------------------------------------------------------------------------
    template <typename TDerived, typename TBase>
    concept RDerivedFrom =
        std::derived_from<std::remove_cvref_t<TDerived>, std::remove_cvref_t<TBase>>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures `TDerived` not is derived from `TBase`.
    /// --------------------------------------------------------------------------------------------
    template <typename TDerived, typename TBase>
    concept RNotDerivedFrom = (!RDerivedFrom<TDerived, TBase>);

    /// --------------------------------------------------------------------------------------------
    /// Ensures `TDerived` is same as or derived from `TBase`.
    /// --------------------------------------------------------------------------------------------
    template <typename TDerived, typename TBase>
    concept RSameOrDerivedFrom = RSameAs<TDerived, TBase> || RDerivedFrom<TDerived, TBase>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures `TDerived` is not same as or derived from `TBase`.
    /// --------------------------------------------------------------------------------------------
    template <typename TDerived, typename TBase>
    concept RNotSameOrDerivedFrom = !RSameOrDerivedFrom<TDerived, TBase>;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////
    //// Object concepts
    ////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    /// --------------------------------------------------------------------------------------------
    /// Ensures `T` is `Constructible` using `args...`.
    /// --------------------------------------------------------------------------------------------
    template <typename T, typename... TArgs>
    concept RConstructible = requires(TArgs&&... args) { T(forward<TArgs>(args)...); };

    /// --------------------------------------------------------------------------------------------
    /// Ensures `T` is `TriviallyConstructible` using `args...`.
    /// --------------------------------------------------------------------------------------------
    template <typename T, typename... TArgs>
    concept RTriviallyConstructible = std::is_trivially_constructible_v<T, TArgs...>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures `T` is `DefaultConstructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RDefaultConstructible = RConstructible<T>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures `T` is `TriviallyDefaultConstructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RTriviallyDefaultConstructible = RTriviallyConstructible<T>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures each type in `Ts...` is `DefaultConstructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename... Ts>
    concept RDefaultConstructibleAll = (RDefaultConstructible<Ts> and ...);

    /// --------------------------------------------------------------------------------------------
    /// Ensures each type in `Ts...` is `TriviallyDefaultConstructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename... Ts>
    concept RTriviallyDefaultConstructibleAll = (RTriviallyDefaultConstructible<Ts> and ...);

    /// --------------------------------------------------------------------------------------------
    /// Ensures `T` is `CopyConstructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RCopyConstructible = RConstructible<T, const T&>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures `T` is `TriviallyCopyConstructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RTriviallyCopyConstructible = RTriviallyConstructible<T, const T&>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures each type in `Ts...` is `CopyConstructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename... Ts>
    concept RCopyConstructibleAll = (RCopyConstructible<Ts> and ...);

    /// --------------------------------------------------------------------------------------------
    /// Ensures each type in `Ts...` is `TriviallyCopyConstructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename... Ts>
    concept RTriviallyCopyConstructibleAll = (RTriviallyCopyConstructible<Ts> and ...);

    /// --------------------------------------------------------------------------------------------
    /// Ensures `T` is `MoveConstructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RMoveConstructible = RConstructible<T, T&&>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures `T` is `TriviallyMoveConstructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RTriviallyMoveConstructible = RTriviallyConstructible<T, T&&>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures each type in `Ts...` is `MoveConstructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename... Ts>
    concept RMoveConstructibleAll = (RMoveConstructible<Ts> and ...);

    /// --------------------------------------------------------------------------------------------
    /// Ensures each type in `Ts...` is `TriviallyMoveConstructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename... Ts>
    concept RTriviallyMoveConstructibleAll = (RTriviallyMoveConstructible<Ts> and ...);

    /// --------------------------------------------------------------------------------------------
    /// Ensures `T` is `DefaultInitializable`.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RDefaultInitializable = requires {
        requires RDefaultConstructible<T>;

        (void)new T;
        T{};
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures each type in `Ts...` is `DefaultInitializable`.
    /// --------------------------------------------------------------------------------------------
    template <typename... Ts>
    concept RDefaultInitializableAll = (RDefaultInitializable<Ts> and ...);

    /// --------------------------------------------------------------------------------------------
    /// Ensures `T0` is `Assignable` using `T1`.
    /// --------------------------------------------------------------------------------------------
    template <typename T0, typename T1>
    concept RAssignable = requires(T0 t0, T1 t1)
    {
        { t0 = forward<T1>(t1) } -> RSameAs<T0&>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures `T` is `TriviallyAssignable` using `from`.
    /// --------------------------------------------------------------------------------------------
    template <typename T0, typename T1>
    concept RTriviallyAssignable = std::is_trivially_assignable_v<T0, T1>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures `T` is `CopyAssignable`.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RCopyAssignable = RAssignable<T, const T&>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures `T` is `TriviallyCopyAssignable`.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RTriviallyCopyAssignable = RTriviallyAssignable<T&, const T&>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures each type in `Ts...` is `CopyAssignable`.
    /// --------------------------------------------------------------------------------------------
    template <typename... Ts>
    concept RCopyAssignableAll = (RCopyAssignable<Ts> and ...);

    /// --------------------------------------------------------------------------------------------
    /// Ensures each type in `Ts...` is `TriviallyCopyAssignable`.
    /// --------------------------------------------------------------------------------------------
    template <typename... Ts>
    concept RTriviallyCopyAssignableAll = (RTriviallyCopyAssignable<Ts> and ...);

    /// --------------------------------------------------------------------------------------------
    /// Ensures `T` is `MoveAssignable`.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RMoveAssignable = RAssignable<T, T&&>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures `T` is `TriviallyMoveAssignable`.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RTriviallyMoveAssignable = RTriviallyAssignable<T&, T&&>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures each type in `Ts...` is `MoveAssignable`.
    /// --------------------------------------------------------------------------------------------
    template <typename... Ts>
    concept RMoveAssignableAll = (RMoveAssignable<Ts> and ...);

    /// --------------------------------------------------------------------------------------------
    /// Ensures each type in `Ts...` is `TriviallyMoveAssignable`.
    /// --------------------------------------------------------------------------------------------
    template <typename... Ts>
    concept RTriviallyMoveAssignableAll = (RTriviallyMoveAssignable<Ts> and ...);

    /// --------------------------------------------------------------------------------------------
    /// Ensures `T` is``CopyConstructible` and `CopyAssignable`.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RCopyable = RCopyConstructible<T> && RCopyAssignable<T>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures `T` is``CopyConstructible` and `TriviallyCopyAssignable`.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RTriviallyCopyable = RTriviallyCopyConstructible<T> && RTriviallyCopyAssignable<T>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures each type in `Ts...` is `Copyable`.
    /// --------------------------------------------------------------------------------------------
    template <typename... Ts>
    concept RCopyableAll = (RCopyable<Ts> and ...);

    /// --------------------------------------------------------------------------------------------
    /// Ensures each type in `Ts...` is `TriviallyCopyable`.
    /// --------------------------------------------------------------------------------------------
    template <typename... Ts>
    concept RTriviallyCopyableAll = (RTriviallyCopyable<Ts> and ...);

    /// --------------------------------------------------------------------------------------------
    /// Ensures `T` is `MoveConstructible` and `MoveAssignable`.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RMoveable = RMoveConstructible<T> && RMoveAssignable<T>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures `T` is `MoveConstructible` and `TriviallyMoveAssignable`.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RTriviallyMoveable = RTriviallyMoveConstructible<T> && RTriviallyMoveAssignable<T>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures each type in `Ts...` is `Moveable`.
    /// --------------------------------------------------------------------------------------------
    template <typename... Ts>
    concept RMoveableAll = (RMoveable<Ts> and ...);

    /// --------------------------------------------------------------------------------------------
    /// Ensures each type in `Ts...` is `TriviallyMoveable`.
    /// --------------------------------------------------------------------------------------------
    template <typename... Ts>
    concept RTriviallyMoveableAll = (RTriviallyMoveable<Ts> and ...);

    /// --------------------------------------------------------------------------------------------
    /// Ensures `T0` is `Swappable` with `T1`.
    /// --------------------------------------------------------------------------------------------
    template <typename T0, typename T1>
    concept RSwappableWith = RAssignable<T0, T1> && RAssignable<T1, T0>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures `T0` is `TriviallySwappable` with `T2`.
    /// --------------------------------------------------------------------------------------------
    template <typename T0, typename T1>
    concept RTriviallySwappableWith = RTriviallyAssignable<T0, T1> && RTriviallyAssignable<T1, T0>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures `T` is `Swappable` with itself.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RSwappable = RSwappableWith<T, T>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures `T` is `TriviallySwappable` with itself.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RTriviallySwappable = RTriviallySwappableWith<T, T>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures `T` is `Destructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RDestructible = std::destructible<T>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures `T` is `TrivallyDestructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RTriviallyDestructible = std::is_trivially_destructible_v<T>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures each type in `Ts...` is `Destructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename... Ts>
    concept RDestructibleAll = (RDestructible<Ts> and ...);

    /// --------------------------------------------------------------------------------------------
    /// Ensures each type in `Ts...` is `TrivallyDestructible`.
    /// --------------------------------------------------------------------------------------------
    template <typename... Ts>
    concept RTriviallyDestructibleAll = (RTriviallyDestructible<Ts> and ...);

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////
    //// Comparision Concepts.
    ////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    /// --------------------------------------------------------------------------------------------
    /// Ensures `T0` and `T1` are `EqualityComparable`.
    /// --------------------------------------------------------------------------------------------
    template <typename T0, typename T1>
    concept REqualityComparableWith = requires(T0 t0, T1 t1)
    {
        { t0 == t1 } -> RConvertibleTo<bool>;
        { t0 != t1 } -> RConvertibleTo<bool>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures `T` is `EqualityComparable`.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept REqualityComparable = REqualityComparableWith<T, T>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures `T0` and `T1` are `Comparable`.
    /// --------------------------------------------------------------------------------------------
    template <typename T0, typename T1>
    concept RComparableWith = requires(T0 t0, T1 t1)
    {
        requires REqualityComparableWith<T0, T1>;

        { t0 < t1 } -> RConvertibleTo<bool>;
        { t0 > t1 } -> RConvertibleTo<bool>;
        { t0 <= t1 } -> RConvertibleTo<bool>;
        { t0 >= t1 } -> RConvertibleTo<bool>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures `T` is `Comparable`.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RComparable = RComparableWith<T, T>;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////
    //// Type Concepts
    ////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    /// --------------------------------------------------------------------------------------------
    /// Ensures `T` is {SemiRegular}.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RSemiRegular = RCopyable<T> && RDefaultInitializable<T>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures `T` is {Regular}.
    /// --------------------------------------------------------------------------------------------
    template <class T>
    concept Regular = RSemiRegular<T> && REqualityComparable<T>;
}
