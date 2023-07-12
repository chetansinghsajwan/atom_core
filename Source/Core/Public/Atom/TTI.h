#pragma once
#include <type_traits>

#include "Atom/Core/LangExtensions.h"
#include "Atom/Core/Compiler.h"

namespace Atom::TTI
{
    namespace Private
    {
        struct Empty { };
    }

    template <bool Condition, tname TTrue, tname TFalse>
    using TConditional = std::conditional_t<Condition, TTrue, TFalse>;

    template <bool Condition, tname TFalse, tname TTrue>
    using TNotConditional = std::conditional_t<Condition, TFalse, TTrue>;

    template <bool Condition, tname T>
    using TConditionalField = TConditional<Condition, T, Private::Empty>;

    template <tname TBase, tname TDerived>
    constexpr bool IsBaseOf = std::is_base_of_v<TBase, std::decay_t<TDerived>>;

    template <tname TBase, tname TDerived>
    constexpr bool IsNotBaseOf = !IsBaseOf<TBase, TDerived>;

    template <bool Requirements, tname T = void>
    using TEnableIf = std::enable_if_t<Requirements, T>;

    template <tname T>
    constexpr bool IsCopyConstructible = std::is_copy_constructible_v<T>;

    template <tname T>
    constexpr bool IsMoveConstructible = std::is_copy_constructible_v<T>;

    template <tname T>
    constexpr bool IsCopyAssignable = std::is_copy_assignable_v<T>;

    template <tname T>
    constexpr bool IsMoveAssignable = std::is_move_assignable_v<T>;

    template <tname T1, tname T2>
    constexpr bool IsSame = std::is_same_v<T1, T2>;

    template <tname T1, tname T2>
    constexpr bool IsNotSame = !IsSame<T1, T2>;

    template <tname T>
    using TRemoveConst = std::remove_const_t<T>;

    template <tname T>
    using TRemoveCVRef = std::remove_cvref_t<T>;
}

/// ------------------------------------------------------------------------------------------------
/// ATOM_ATTR_NO_UNIQUE_ADDRESS
/// ------------------------------------------------------------------------------------------------
#if defined(ATOM_COMPILER_CLANG)
    #define ATOM_ATTR_NO_UNIQUE_ADDRESS

#elif defined(ATOM_COMPILER_MSVC) 
    #if (ATOM_COMPILER_MSVC_VER >= 1929)
        #define ATOM_ATTR_NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
    #else
        #define ATOM_ATTR_NO_UNIQUE_ADDRESS [[no_unique_address]]
    #endif

#else
    #define ATOM_ATTR_NO_UNIQUE_ADDRESS [[no_unique_address]]

#endif

/// ------------------------------------------------------------------------------------------------
/// ATOM_CONDITIONAL_FIELD
/// ------------------------------------------------------------------------------------------------
#define ATOM_CONDITIONAL_FIELD(Condition, T) \
    ATOM_ATTR_NO_UNIQUE_ADDRESS \
    ::Atom::TTI::TConditionalField<(Condition), T>
