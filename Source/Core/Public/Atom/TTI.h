#pragma once
#include <type_traits>

#include "Atom/Core.h"

namespace Atom
{
    namespace TTI
    {
        template <bool Condition, typename TTrue, typename TFalse>
        using TConditional = std::conditional_t<Condition, TTrue, TFalse>;

        template <typename TBase, typename TDerived>
        constexpr bool IsBaseOf = std::is_base_of_v<TBase, std::decay_t<TDerived>>;

        template <typename TBase, typename TDerived>
        constexpr bool IsNotBaseOf = !IsBaseOf<TBase, TDerived>;

        template <typename TFrom, typename TTo>
        concept RConvertible = std::convertible_to<TFrom, TTo>;

        template <typename TLeft, typename TRight>
        concept RSameAs = std::same_as<TLeft, TRight>;

        template <typename TLeft, typename TRight>
        concept RNotSameAs = !std::same_as<TLeft, TRight>;

        template <bool Requirements>
        using TEnableIf = std::enable_if_t<Requirements>;

        template <typename T>
        constexpr bool IsCopyConstructible = std::is_copy_constructible_v<T>;

        template <typename T>
        constexpr bool IsMoveConstructible = std::is_copy_constructible_v<T>;

        template <typename T>
        constexpr bool IsCopyAssignable = std::is_copy_assignable_v<T>;

        template <typename T>
        constexpr bool IsMoveAssignable = std::is_move_assignable_v<T>;

        template <typename T1, typename T2>
        constexpr bool IsSame = std::is_same_v<T1, T2>;

        template <typename T1, typename T2>
        constexpr bool IsNotSame = !IsSame<T1, T2>;
    }
}