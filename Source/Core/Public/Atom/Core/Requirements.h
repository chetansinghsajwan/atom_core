#pragma once
#include "Atom/TTI.h"
#include "Primitives.h"

#include <concepts>
#include <type_traits>

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// @TODO: Needs refactoring.
    /// --------------------------------------------------------------------------------------------
    template <typename TCheck, typename T, typename... Ts>
    class _CheckAllImpl
    {
    public:
        static constexpr bool Value =
            TCheck().template operator()<T>() && _CheckAllImpl<TCheck, Ts...>::Value;
    };

    template <typename TCheck, typename T>
    class _CheckAllImpl<TCheck, T>
    {
    public:
        static constexpr bool Value = TCheck().template operator()<T>();
    };

    template <typename TCheck, typename T, typename... Ts>
    constexpr bool _CheckAll = _CheckAllImpl<TCheck, T, Ts...>::Value;

#define _WRAP_REQ(...) decltype([]<typename T> { return __VA_ARGS__; })

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////
    //// Basic Concepts
    ////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T0} is same as {T1}.
    /// --------------------------------------------------------------------------------------------
    template <typename T0, typename T1>
    concept RSameAs = std::same_as<T0, T1>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T0} is {void}.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RIsVoid = RSameAs<T, void>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures unqualified type of {T0} is same as unqualified type of {T1}.
    /// --------------------------------------------------------------------------------------------
    template <typename T0, typename T1>
    concept RSameAsUnqualified = std::same_as<TTI::TRemoveCVRef<T0>, TTI::TRemoveCVRef<T1>>;

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
    /// Ensures {TFrom} is {Convertible} to {TTo}.
    /// --------------------------------------------------------------------------------------------
    template <typename TFrom, typename TTo>
    concept RConvertibleTo = requires(TFrom from) { static_cast<TTo>(from); };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TDerived} is derived from {TBase}.
    /// --------------------------------------------------------------------------------------------
    template <typename TDerived, typename TBase>
    concept RDerivedFrom =
        std::derived_from<std::remove_cvref_t<TDerived>, std::remove_cvref_t<TBase>>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TDerived} not is derived from {TBase}.
    /// --------------------------------------------------------------------------------------------
    template <typename TDerived, typename TBase>
    concept RNotDerivedFrom = (!RDerivedFrom<TDerived, TBase>);

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TDerived} is same as or derived from {TBase}.
    /// --------------------------------------------------------------------------------------------
    template <typename TDerived, typename TBase>
    concept RSameOrDerivedFrom = RSameAs<TDerived, TBase> || RDerivedFrom<TDerived, TBase>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TDerived} is not same as or derived from {TBase}.
    /// --------------------------------------------------------------------------------------------
    template <typename TDerived, typename TBase>
    concept RNotSameOrDerivedFrom = !RSameOrDerivedFrom<TDerived, TBase>;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////
    //// Object concepts
    ////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {Constructible} using {args...}.
    /// --------------------------------------------------------------------------------------------
    template <typename T, typename... TArgs>
    concept RConstructible = requires(TArgs&&... args) { T(fwd(args)...); };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {TriviallyConstructible} using {args...}.
    /// --------------------------------------------------------------------------------------------
    template <typename T, typename... TArgs>
    concept RTriviallyConstructible = std::is_trivially_constructible_v<T, TArgs...>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {DefaultConstructible}.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RDefaultConstructible = RConstructible<T>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {TriviallyDefaultConstructible}.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RTriviallyDefaultConstructible = RTriviallyConstructible<T>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures each {T} in {Ts...} is {DefaultConstructible}.
    /// --------------------------------------------------------------------------------------------
    template <typename... Ts>
    concept RDefaultConstructibleAll = _CheckAll<_WRAP_REQ(RDefaultConstructible<T>), Ts...>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures each {T} in {Ts...} is {TriviallyDefaultConstructible}.
    /// --------------------------------------------------------------------------------------------
    template <typename... Ts>
    concept RTriviallyDefaultConstructibleAll =
        _CheckAll<_WRAP_REQ(RTriviallyDefaultConstructible<T>), Ts...>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {CopyConstructible}.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RCopyConstructible = RConstructible<T, const T&>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {TriviallyCopyConstructible}.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RTriviallyCopyConstructible = RTriviallyConstructible<T, const T&>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures each {T} in {Ts...} is {CopyConstructible}.
    /// --------------------------------------------------------------------------------------------
    template <typename... Ts>
    concept RCopyConstructibleAll = _CheckAll<_WRAP_REQ(RCopyConstructible<T>), Ts...>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures each {T} in {Ts...} is {TriviallyCopyConstructible}.
    /// --------------------------------------------------------------------------------------------
    template <typename... Ts>
    concept RTriviallyCopyConstructibleAll =
        _CheckAll<_WRAP_REQ(RTriviallyCopyConstructible<T>), Ts...>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {MoveConstructible}.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RMoveConstructible = RConstructible<T, T&&>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {TriviallyMoveConstructible}.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RTriviallyMoveConstructible = RTriviallyConstructible<T, T&&>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures each {T} in {Ts...} is {MoveConstructible}.
    /// --------------------------------------------------------------------------------------------
    template <typename... Ts>
    concept RMoveConstructibleAll = _CheckAll<_WRAP_REQ(RMoveConstructible<T>), Ts...>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures each {T} in {Ts...} is {TriviallyMoveConstructible}.
    /// --------------------------------------------------------------------------------------------
    template <typename... Ts>
    concept RTriviallyMoveConstructibleAll =
        _CheckAll<_WRAP_REQ(RTriviallyMoveConstructible<T>), Ts...>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {DefaultInitializable}.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RDefaultInitializable = requires {
        requires RDefaultConstructible<T>;

        (void)new T;
        T{};
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures each {T} in {Ts...} is {DefaultInitializable}.
    /// --------------------------------------------------------------------------------------------
    template <typename... Ts>
    concept RDefaultInitializableAll = _CheckAll<_WRAP_REQ(RDefaultInitializable<T>), Ts...>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {Assignable} using {from}.
    /// --------------------------------------------------------------------------------------------
    template <typename T0, typename T1>
    concept RAssignable = requires(T0 t1, T1 t2) {
        {
            t1 = fwd(t2)
        } -> RSameAs<T0&>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {TriviallyAssignable} using {from}.
    /// --------------------------------------------------------------------------------------------
    template <typename T0, typename T1>
    concept RTriviallyAssignable = std::is_trivially_assignable_v<T0, T1>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {CopyAssignable}.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RCopyAssignable = RAssignable<T, const T&>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {TriviallyCopyAssignable}.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RTriviallyCopyAssignable = RTriviallyAssignable<T&, const T&>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures each {T} in {Ts...} is {CopyAssignable}.
    /// --------------------------------------------------------------------------------------------
    template <typename... Ts>
    concept RCopyAssignableAll = _CheckAll<_WRAP_REQ(RCopyAssignable<T>), Ts...>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures each {T} in {Ts...} is {TriviallyCopyAssignable}.
    /// --------------------------------------------------------------------------------------------
    template <typename... Ts>
    concept RTriviallyCopyAssignableAll = _CheckAll<_WRAP_REQ(RTriviallyCopyAssignable<T>), Ts...>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {MoveAssignable}.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RMoveAssignable = RAssignable<T, T&&>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {TriviallyMoveAssignable}.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RTriviallyMoveAssignable = RTriviallyAssignable<T&, T&&>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures each {T} in {Ts...} is {MoveAssignable}.
    /// --------------------------------------------------------------------------------------------
    template <typename... Ts>
    concept RMoveAssignableAll = _CheckAll<_WRAP_REQ(RMoveAssignable<T>), Ts...>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures each {T} in {Ts...} is {TriviallyMoveAssignable}.
    /// --------------------------------------------------------------------------------------------
    template <typename... Ts>
    concept RTriviallyMoveAssignableAll = _CheckAll<_WRAP_REQ(RTriviallyMoveAssignable<T>), Ts...>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {CopyConstructible} and {CopyAssignable}.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RCopyable = RCopyConstructible<T> && RCopyAssignable<T>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {CopyConstructible} and {TriviallyCopyAssignable}.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RTriviallyCopyable = RTriviallyCopyConstructible<T> && RTriviallyCopyAssignable<T>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures each {T} in {Ts...} is {Copyable}.
    /// --------------------------------------------------------------------------------------------
    template <typename... Ts>
    concept RCopyableAll = _CheckAll<_WRAP_REQ(RCopyable<T>), Ts...>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures each {T} in {Ts...} is {TriviallyCopyable}.
    /// --------------------------------------------------------------------------------------------
    template <typename... Ts>
    concept RTriviallyCopyableAll = _CheckAll<_WRAP_REQ(RTriviallyCopyable<T>), Ts...>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {MoveConstructible} and {MoveAssignable}.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RMoveable = RMoveConstructible<T> && RMoveAssignable<T>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {MoveConstructible} and {TriviallyMoveAssignable}.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RTriviallyMoveable = RTriviallyMoveConstructible<T> && RTriviallyMoveAssignable<T>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures each {T} in {Ts...} is {Moveable}.
    /// --------------------------------------------------------------------------------------------
    template <typename... Ts>
    concept RMoveableAll = _CheckAll<_WRAP_REQ(RMoveable<T>), Ts...>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures each {T} in {Ts...} is {TriviallyMoveable}.
    /// --------------------------------------------------------------------------------------------
    template <typename... Ts>
    concept RTriviallyMoveableAll = _CheckAll<_WRAP_REQ(RTriviallyMoveable<T>), Ts...>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T0} is {Swappable} with {T1}.
    /// --------------------------------------------------------------------------------------------
    template <typename T0, typename T1>
    concept RSwappableWith = RAssignable<T0, T1> && RAssignable<T1, T0>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T0} is {Swappable} with {TriviallyT2}.
    /// --------------------------------------------------------------------------------------------
    template <typename T0, typename T1>
    concept RTriviallySwappableWith = RTriviallyAssignable<T0, T1> && RTriviallyAssignable<T1, T0>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {Swappable} with itself.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RSwappable = RSwappableWith<T, T>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {Swappable} Triviallywith itself.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RTriviallySwappable = RTriviallySwappableWith<T, T>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {Destructible}.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RDestructible = std::destructible<T>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {TrivallyDestructible}.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RTriviallyDestructible = std::is_trivially_destructible_v<T>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures each {T} in {Ts...} is {Destructible}.
    /// --------------------------------------------------------------------------------------------
    template <typename... Ts>
    concept RDestructibleAll = _CheckAll<_WRAP_REQ(RDestructible<T>), Ts...>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures each {T} in {Ts...} is {TrivallyDestructible}.
    /// --------------------------------------------------------------------------------------------
    template <typename... Ts>
    concept RTriviallyDestructibleAll = _CheckAll<_WRAP_REQ(RTriviallyDestructible<T>), Ts...>;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////
    //// Comparision Concepts.
    ////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T0} and {T1} are {EqualityComparable}.
    /// --------------------------------------------------------------------------------------------
    template <typename T0, typename T1>
    concept REqualityComparableWith = requires(T0 t1, T1 t2) {
        {
            t1 == t2
        } -> RConvertibleTo<bool>;
        {
            t1 != t2
        } -> RConvertibleTo<bool>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {EqualityComparable}.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept REqualityComparable = REqualityComparableWith<T, T>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T0} and {T1} are {Comparable}.
    /// --------------------------------------------------------------------------------------------
    template <typename T0, typename T1>
    concept RComparableWith = requires(T0 t1, T1 t2) {
        requires REqualityComparableWith<T0, T1>;

        {
            t1 < t2
        } -> RConvertibleTo<bool>;
        {
            t1 > t2
        } -> RConvertibleTo<bool>;
        {
            t1 <= t2
        } -> RConvertibleTo<bool>;
        {
            t1 >= t2
        } -> RConvertibleTo<bool>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {Comparable}.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RComparable = RComparableWith<T, T>;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////
    //// Type Concepts
    ////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {SemiRegular}.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RSemiRegular = RCopyable<T> && RDefaultInitializable<T>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {Regular}.
    /// --------------------------------------------------------------------------------------------
    template <class T>
    concept Regular = RSemiRegular<T> && REqualityComparable<T>;
}
