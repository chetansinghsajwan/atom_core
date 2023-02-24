#pragma once
#include <type_traits>

#include "Atom/Core.h"

namespace Atom
{
    namespace TTI
    {
        template <bool condition, typename TrueType, typename FalseType>
        using ConditionalType = std::conditional_t<condition, TrueType, FalseType>;

        template <typename Base, typename Derived>
        constexpr bool IsBaseOf = std::is_base_of_v<Base, std::decay_t<Derived>>;

        template <typename Base, typename Derived>
        constexpr bool IsNotBaseOf = !IsBaseOf<Base, Derived>;

        template <typename From, typename To>
        concept ConvertibleTo = std::convertible_to<From, To>;

        template <typename LHS, typename RHS>
        concept SameAs = std::same_as<LHS, RHS>;

        template <typename LHS, typename RHS>
        concept NotSameAs = !std::same_as<LHS, RHS>;

        template <bool Requirements>
        using EnableIf = std::enable_if_t<Requirements>;

        template <typename T>
        constexpr bool IsCopyConstructible = std::is_copy_constructible_v<T>;

        template <typename T>
        constexpr bool IsMoveConstructible = std::is_copy_constructible_v<T>;

        template <typename T>
        constexpr bool IsCopyAssignable = std::is_copy_assignable_v<T>;

        template <typename T>
        constexpr bool IsMoveAssignable = std::is_move_assignable_v<T>;

        template <typename T, typename U>
        constexpr bool IsSame = std::is_same_v<T, U>;

        template <typename T, typename U>
        constexpr bool IsNotSame = !IsSame<T, U>;
    }
}