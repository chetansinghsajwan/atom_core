#pragma once
#include "Primitives.h"
#include "Atom/TTI.h"

#include <concepts>
#include <type_traits>

namespace Atom
{
//// -----------------------------------------------------------------------------------------------
//// Basic Concepts
//// -----------------------------------------------------------------------------------------------

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T1} is same as {T2}.
    /// --------------------------------------------------------------------------------------------
    template <typename T1, typename T2>
    concept RSameAs = std::same_as<T1, T2>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures unqualified type of {T1} is same as unqualified type of {T2}.
    /// --------------------------------------------------------------------------------------------
    template <typename T1, typename T2>
    concept RSameAsUnqualified = std::same_as<TTI::TRemoveCVRef<T1>, TTI::TRemoveCVRef<T2>>;

    template <bool V>
    concept RTrue = (V == true);

    template <bool V>
    concept RFalse = (V == false);

    template <typename T>
    concept RConst = ::std::is_const_v<T>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TFrom} is {Convertible} to {TTo}.
    /// --------------------------------------------------------------------------------------------
    template <typename TFrom, typename TTo>
    concept RConvertibleTo = requires
    {
        static_cast<TTo>(declval(TFrom));
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {TDerived} is derived from {TBase}.
    /// --------------------------------------------------------------------------------------------
    template <typename TDerived, typename TBase>
    concept RDerivedFrom = ::std::derived_from<::std::remove_cvref_t<TDerived>, ::std::remove_cvref_t<TBase>>;

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

//// -----------------------------------------------------------------------------------------------
//// Object concepts 
//// -----------------------------------------------------------------------------------------------

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {Constructible} using {args...}.
    /// --------------------------------------------------------------------------------------------
    template <typename T, typename... TArgs>
    concept RConstructible = requires(TArgs&&... args)
    {
        T(FORWARD(args)...);
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {DefaultConstructible}.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RDefaultConstructible = RConstructible<T>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {CopyConstructible}.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RCopyConstructible = RConstructible<T, const T&>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {MoveConstructible}.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RMoveConstructible = RConstructible<T, T&&>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {DefaultInitializable}.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RDefaultInitializable = requires
    {
        requires RDefaultConstructible<T>;

    (void) new T;
    T{};
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {Assignable} using {from}.
    /// --------------------------------------------------------------------------------------------
    template <typename T1, typename T2>
    concept RAssignable = requires(T1 t1, T2 t2)
    {
        { t1 = t2 } -> RSameAs<T1&>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {CopyAssignable}.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RCopyAssignable = RAssignable<T, const T&>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {MoveAssignable}.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RMoveAssignable = RAssignable<T, T&&>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {CopyConstructible} and {CopyAssignable}.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RCopyable = RCopyConstructible<T> && RCopyAssignable<T>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {MoveConstructible} and {MoveAssignable}.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RMoveable = RMoveConstructible<T> && RMoveAssignable<T>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {Destructible}.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept RDestructible = requires
    {
        requires std::destructible<T>;
    };

//// -----------------------------------------------------------------------------------------------    
//// Comparision Concepts.
//// -----------------------------------------------------------------------------------------------    

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T1} and {T2} are {EqualityComparable}.
    /// --------------------------------------------------------------------------------------------
    template <typename T1, typename T2>
    concept REqualityComparableWith = requires(T1 t1, T2 t2)
    {
        { t1 == t2 } -> RConvertibleTo<bool>;
        { t1 != t2 } -> RConvertibleTo<bool>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T} is {EqualityComparable}.
    /// --------------------------------------------------------------------------------------------
    template <typename T>
    concept REqualityComparable = REqualityComparableWith<T, T>;

    /// --------------------------------------------------------------------------------------------
    /// Ensures {T1} and {T2} are {Comparable}.
    /// --------------------------------------------------------------------------------------------
    template <typename T1, typename T2>
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
    template <typename T>
    concept RComparable = RComparableWith<T, T>;

//// -----------------------------------------------------------------------------------------------    
//// Type Concepts
//// -----------------------------------------------------------------------------------------------    

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