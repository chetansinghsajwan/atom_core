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
        constexpr bool IsBaseOf = std::is_base_of_v<Base, Derived>;

        template <typename From, typename To>
        concept ConvertibleTo = std::convertible_to<From, To>;

        template <typename LHS, typename RHS>
        concept SameAs = std::same_as<LHS, RHS>;
    }
}