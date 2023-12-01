#pragma once
#include <type_traits>

#include "Atom/Core/BuildConfig.h"
#include "Atom/Core/LangExtensions.h"

namespace Atom::TTI
{
    class _Empty
    {};

    template <bool Condition, typename TTrue, typename TFalse>
    using TConditional = std::conditional_t<Condition, TTrue, TFalse>;

    template <bool Condition, typename TTrue, typename TFalse = _Empty>
    using TConditionalField = TConditional<Condition, TTrue, TFalse>;

    template <typename TBase, typename TDerived>
    constexpr bool IsBaseOf = std::is_base_of_v<TBase, std::decay_t<TDerived>>;

    template <typename TBase, typename TDerived>
    constexpr bool IsNotBaseOf = !IsBaseOf<TBase, TDerived>;

    template <bool Requirements, typename T = void>
    using TEnableIf = std::enable_if_t<Requirements, T>;

    template <typename T>
    constexpr bool IsCopyConstructible = std::is_copy_constructible_v<T>;

    template <typename T>
    constexpr bool IsMoveConstructible = std::is_copy_constructible_v<T>;

    template <typename T>
    constexpr bool IsCopyAssignable = std::is_copy_assignable_v<T>;

    template <typename T>
    constexpr bool IsMoveAssignable = std::is_move_assignable_v<T>;

    template <typename T0, typename T1>
    constexpr bool IsSame = std::is_same_v<T0, T1>;

    template <typename T0, typename T1>
    constexpr bool IsNotSame = !IsSame<T0, T1>;

    template <typename T>
    using TRemoveConst = std::remove_const_t<T>;

    template <typename T>
    using TRemoveCVRef = std::remove_cvref_t<T>;

    template <typename T>
    using TRemoveQuailfiersRef = std::remove_cvref_t<T>;

    template <typename T>
    using TUnqualified = std::remove_cv_t<T>;

    template <typename T>
    constexpr bool IsVoid = std::is_void_v<T>;

    template <typename T>
    constexpr bool IsRef = std::is_reference_v<T>;

    template <typename T>
    constexpr bool IsRValueRef = std::is_rvalue_reference_v<T>;

    template <typename T>
    constexpr bool IsConst = std::is_const_v<T>;

    template <typename T>
    constexpr bool IsVolatile = std::is_volatile_v<T>;

    template <typename T>
    constexpr bool IsQualified = IsConst<T> || IsVolatile<T>;

    template <typename T>
    using TPure = TRemoveQuailfiersRef<T>;

    template <typename T>
    constexpr bool IsPure = not IsQualified<T> and not IsRef<T>;

    template <typename T>
    constexpr bool IsEmpty = std::is_empty_v<T>;
}
