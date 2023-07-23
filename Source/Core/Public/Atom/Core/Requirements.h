#pragma once
#include "Primitives.h"
#include "Atom/TTI.h"

#include <concepts>
#include <type_traits>

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// @TODO: Needs refactoring.
    /// --------------------------------------------------------------------------------------------
    template <tname TCheck, tname T, tname... Ts>
    class _CheckAllImpl
    {
        pub static cexpr bool Value = TCheck().template op()<T>() && 
            _CheckAllImpl<TCheck, Ts...>::Value;
    };

    template <tname TCheck, tname T>
    class _CheckAllImpl<TCheck, T>
    {
        pub static cexpr bool Value = TCheck().template op()<T>();
    };

    template <tname TCheck, tname T, tname... Ts>
    cexpr bool _CheckAll = _CheckAllImpl<TCheck, T, Ts...>::Value;

#define _WRAP_REQ(...) decltype([]<tname T> { return __VA_ARGS__; })

//// -----------------------------------------------------------------------------------------------
//// Basic Concepts
//// -----------------------------------------------------------------------------------------------

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T1} is same as {T2}.
    /// --------------------------------------------------------------------------------------------
    template <tname T1, tname T2>
    concept RSameAs = std::same_as<T1, T2>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures unqualified type of {T1} is same as unqualified type of {T2}.
    /// --------------------------------------------------------------------------------------------
    template <tname T1, tname T2>
    concept RSameAsUnqualified = std::same_as<TTI::TRemoveCVRef<T1>, TTI::TRemoveCVRef<T2>>;

    template <bool V>
    concept RTrue = (V == true);

    template <bool V>
    concept RFalse = (V == false);

    template <tname T>
    concept RConst = std::is_const_v<T>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TFrom} is {Convertible} to {TTo}.
    /// --------------------------------------------------------------------------------------------
    template <tname TFrom, tname TTo>
    concept RConvertibleTo = requires
    {
        static_cast<TTo>(declval(TFrom));
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TDerived} is derived from {TBase}.
    /// --------------------------------------------------------------------------------------------
    template <tname TDerived, tname TBase>
    concept RDerivedFrom = std::derived_from<std::remove_cvref_t<TDerived>, 
        std::remove_cvref_t<TBase>>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TDerived} not is derived from {TBase}.
    /// --------------------------------------------------------------------------------------------
    template <tname TDerived, tname TBase>
    concept RNotDerivedFrom = (!RDerivedFrom<TDerived, TBase>);

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TDerived} is same as or derived from {TBase}.
    /// --------------------------------------------------------------------------------------------
    template <tname TDerived, tname TBase>
    concept RSameOrDerivedFrom = RSameAs<TDerived, TBase> || RDerivedFrom<TDerived, TBase>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TDerived} is not same as or derived from {TBase}.
    /// --------------------------------------------------------------------------------------------
    template <tname TDerived, tname TBase>
    concept RNotSameOrDerivedFrom = !RSameOrDerivedFrom<TDerived, TBase>;

//// -----------------------------------------------------------------------------------------------
//// Object concepts 
//// -----------------------------------------------------------------------------------------------

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {Constructible} using {args...}.
    /// --------------------------------------------------------------------------------------------
    template <tname T, tname... TArgs>
    concept RConstructible = requires(TArgs&&... args)
    {
        T(fwd(args)...);
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {TriviallyConstructible} using {args...}.
    /// --------------------------------------------------------------------------------------------
    template <tname T, tname... TArgs>
    concept RTriviallyConstructible = std::is_trivially_constructible_v<T, TArgs...>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {DefaultConstructible}.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    concept RDefaultConstructible = RConstructible<T>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {TriviallyDefaultConstructible}.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    concept RTriviallyDefaultConstructible = RTriviallyConstructible<T>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures each {T} in {Ts...} is {DefaultConstructible}.
    /// --------------------------------------------------------------------------------------------
    template <tname... Ts>
    concept RDefaultConstructibleAll = _CheckAll<_WRAP_REQ(RDefaultConstructible<T>), Ts...>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures each {T} in {Ts...} is {TriviallyDefaultConstructible}.
    /// --------------------------------------------------------------------------------------------
    template <tname... Ts>
    concept RTriviallyDefaultConstructibleAll = 
        _CheckAll<_WRAP_REQ(RTriviallyDefaultConstructible<T>), Ts...>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {CopyConstructible}.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    concept RCopyConstructible = RConstructible<T, const T&>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {TriviallyCopyConstructible}.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    concept RTriviallyCopyConstructible = RTriviallyConstructible<T, const T&>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures each {T} in {Ts...} is {CopyConstructible}.
    /// --------------------------------------------------------------------------------------------
    template <tname... Ts>
    concept RCopyConstructibleAll = _CheckAll<_WRAP_REQ(RCopyConstructible<T>), Ts...>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures each {T} in {Ts...} is {TriviallyCopyConstructible}.
    /// --------------------------------------------------------------------------------------------
    template <tname... Ts>
    concept RTriviallyCopyConstructibleAll = 
        _CheckAll<_WRAP_REQ(RTriviallyCopyConstructible<T>), Ts...>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {MoveConstructible}.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    concept RMoveConstructible = RConstructible<T, T&&>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {TriviallyMoveConstructible}.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    concept RTriviallyMoveConstructible = RTriviallyConstructible<T, T&&>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures each {T} in {Ts...} is {MoveConstructible}.
    /// --------------------------------------------------------------------------------------------
    template <tname... Ts>
    concept RMoveConstructibleAll = _CheckAll<_WRAP_REQ(RMoveConstructible<T>), Ts...>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures each {T} in {Ts...} is {TriviallyMoveConstructible}.
    /// --------------------------------------------------------------------------------------------
    template <tname... Ts>
    concept RTriviallyMoveConstructibleAll = 
        _CheckAll<_WRAP_REQ(RTriviallyMoveConstructible<T>), Ts...>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {DefaultInitializable}.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    concept RDefaultInitializable = requires
    {
        requires RDefaultConstructible<T>;

        (void) new T;
        T{};
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures each {T} in {Ts...} is {DefaultInitializable}.
    /// --------------------------------------------------------------------------------------------
    template <tname... Ts>
    concept RDefaultInitializableAll = _CheckAll<_WRAP_REQ(RDefaultInitializable<T>), Ts...>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {Assignable} using {from}.
    /// --------------------------------------------------------------------------------------------
    template <tname T1, tname T2>
    concept RAssignable = requires(T1 t1, T2 t2)
    {
        { t1 = t2 } -> RSameAs<T1&>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {TriviallyAssignable} using {from}.
    /// --------------------------------------------------------------------------------------------
    template <tname T1, tname T2>
    concept RTriviallyAssignable = std::is_trivially_assignable_v<T1, T2>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {CopyAssignable}.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    concept RCopyAssignable = RAssignable<T, const T&>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {TriviallyCopyAssignable}.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    concept RTriviallyCopyAssignable = RTriviallyAssignable<T, const T&>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures each {T} in {Ts...} is {CopyAssignable}.
    /// --------------------------------------------------------------------------------------------
    template <tname... Ts>
    concept RCopyAssignableAll = _CheckAll<_WRAP_REQ(RCopyAssignable<T>), Ts...>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures each {T} in {Ts...} is {TriviallyCopyAssignable}.
    /// --------------------------------------------------------------------------------------------
    template <tname... Ts>
    concept RTriviallyCopyAssignableAll = _CheckAll<_WRAP_REQ(RTriviallyCopyAssignable<T>), Ts...>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {MoveAssignable}.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    concept RMoveAssignable = RAssignable<T, T&&>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {TriviallyMoveAssignable}.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    concept RTriviallyMoveAssignable = RTriviallyAssignable<T, T&&>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures each {T} in {Ts...} is {MoveAssignable}.
    /// --------------------------------------------------------------------------------------------
    template <tname... Ts>
    concept RMoveAssignableAll = _CheckAll<_WRAP_REQ(RMoveAssignable<T>), Ts...>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures each {T} in {Ts...} is {TriviallyMoveAssignable}.
    /// --------------------------------------------------------------------------------------------
    template <tname... Ts>
    concept RTriviallyMoveAssignableAll = _CheckAll<_WRAP_REQ(RTriviallyMoveAssignable<T>), Ts...>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {CopyConstructible} and {CopyAssignable}.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    concept RCopyable = RCopyConstructible<T> && RCopyAssignable<T>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {CopyConstructible} and {TriviallyCopyAssignable}.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    concept RTriviallyCopyable = RTriviallyCopyConstructible<T> && RTriviallyCopyAssignable<T>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures each {T} in {Ts...} is {Copyable}.
    /// --------------------------------------------------------------------------------------------
    template <tname... Ts>
    concept RCopyableAll = _CheckAll<_WRAP_REQ(RCopyable<T>), Ts...>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures each {T} in {Ts...} is {TriviallyCopyable}.
    /// --------------------------------------------------------------------------------------------
    template <tname... Ts>
    concept RTriviallyCopyableAll = _CheckAll<_WRAP_REQ(RTriviallyCopyable<T>), Ts...>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {MoveConstructible} and {MoveAssignable}.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    concept RMoveable = RMoveConstructible<T> && RMoveAssignable<T>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {MoveConstructible} and {TriviallyMoveAssignable}.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    concept RTriviallyMoveable = RTriviallyMoveConstructible<T> && RTriviallyMoveAssignable<T>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures each {T} in {Ts...} is {Moveable}.
    /// --------------------------------------------------------------------------------------------
    template <tname... Ts>
    concept RMoveableAll = _CheckAll<_WRAP_REQ(RMoveable<T>), Ts...>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures each {T} in {Ts...} is {TriviallyMoveable}.
    /// --------------------------------------------------------------------------------------------
    template <tname... Ts>
    concept RTriviallyMoveableAll = _CheckAll<_WRAP_REQ(RTriviallyMoveable<T>), Ts...>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T1} is {Swappable} with {T2}.
    /// --------------------------------------------------------------------------------------------
    template <tname T1, tname T2>
    concept RSwappableWith = RAssignable<T1, T2> && RAssignable<T2, T1>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T1} is {Swappable} with {TriviallyT2}.
    /// --------------------------------------------------------------------------------------------
    template <tname T1, tname T2>
    concept RTriviallySwappableWith = RTriviallyAssignable<T1, T2> && RTriviallyAssignable<T2, T1>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {Swappable} with itself.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    concept RSwappable = RSwappableWith<T, T>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {Swappable} Triviallywith itself.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    concept RTriviallySwappable = RTriviallySwappableWith<T, T>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {Destructible}.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    concept RDestructible = std::destructible<T>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {TrivallyDestructible}.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    concept RTriviallyDestructible = std::is_trivially_destructible_v<T>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures each {T} in {Ts...} is {Destructible}.
    /// --------------------------------------------------------------------------------------------
    template <tname... Ts>
    concept RDestructibleAll = _CheckAll<_WRAP_REQ(RDestructible<T>), Ts...>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures each {T} in {Ts...} is {TrivallyDestructible}.
    /// --------------------------------------------------------------------------------------------
    template <tname... Ts>
    concept RTriviallyDestructibleAll = _CheckAll<_WRAP_REQ(RTriviallyDestructible<T>), Ts...>;

//// -----------------------------------------------------------------------------------------------
//// Comparision Concepts.
//// -----------------------------------------------------------------------------------------------

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T1} and {T2} are {EqualityComparable}.
    /// --------------------------------------------------------------------------------------------
    template <tname T1, tname T2>
    concept REqualityComparableWith = requires(T1 t1, T2 t2)
    {
        { t1 == t2 } -> RConvertibleTo<bool>;
        { t1 != t2 } -> RConvertibleTo<bool>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {EqualityComparable}.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    concept REqualityComparable = REqualityComparableWith<T, T>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T1} and {T2} are {Comparable}.
    /// --------------------------------------------------------------------------------------------
    template <tname T1, tname T2>
    concept RComparableWith = requires(T1 t1, T2 t2)
    {
        requires REqualityComparableWith<T1, T2>;

        { t1 < t2 }  -> RConvertibleTo<bool>;
        { t1 > t2 }  -> RConvertibleTo<bool>;
        { t1 <= t2 } -> RConvertibleTo<bool>;
        { t1 >= t2 } -> RConvertibleTo<bool>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {Comparable}.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    concept RComparable = RComparableWith<T, T>;

//// -----------------------------------------------------------------------------------------------
//// Type Concepts
//// -----------------------------------------------------------------------------------------------

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {SemiRegular}.
    /// --------------------------------------------------------------------------------------------
    template <tname T>
    concept RSemiRegular = RCopyable<T> && RDefaultInitializable<T>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {Regular}.
    /// --------------------------------------------------------------------------------------------
    template <class T>
    concept Regular = RSemiRegular<T> && REqualityComparable<T>;
}