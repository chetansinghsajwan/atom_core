#pragma once
#include <type_traits>

#include "Atom/Core/Compiler.h"
#include "Atom/Core/LangExtensions.h"

namespace Atom::TTI
{
    namespace Private
    {
        class Empty
        {};
    }

    template <bool Condition, typename TTrue, typename TFalse>
    using TConditional = std::conditional_t<Condition, TTrue, TFalse>;

    template <bool Condition, typename TFalse, typename TTrue>
    using TNotConditional = std::conditional_t<Condition, TFalse, TTrue>;

    template <bool Condition, typename T>
    using TConditionalField = TConditional<Condition, T, Private::Empty>;

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

    template <typename T1, typename T2>
    constexpr bool IsSame = std::is_same_v<T1, T2>;

    template <typename T1, typename T2>
    constexpr bool IsNotSame = !IsSame<T1, T2>;

    template <typename T>
    using TRemoveConst = std::remove_const_t<T>;

    template <typename T>
    using TRemoveCVRef = std::remove_cvref_t<T>;

    template <typename T>
    using TRemoveQuailfiersRef = std::remove_cvref_t<T>;

    template <typename T>
    using TUnqualified = std::remove_cv_t<T>;

    template <typename T>
    constexpr bool IsRValueRef = std::is_rvalue_reference_v<T>;

    template <typename T>
    constexpr bool IsConst = std::is_const_v<T>;

    template <typename T>
    constexpr bool IsVolatile = std::is_volatile_v<T>;

    template <typename T>
    constexpr bool IsQualified = IsConst<T> || IsVolatile<T>;
}

/// ------------------------------------------------------------------------------------------------
/// ATOM_ATTR_NO_UNIQUE_ADDRESS
/// ------------------------------------------------------------------------------------------------
#if defined(ATOM_COMPILER_CLANG)
#    define ATOM_ATTR_NO_UNIQUE_ADDRESS

#elif defined(ATOM_COMPILER_MSVC)
#    if (ATOM_COMPILER_MSVC_VER >= 1929)
#        define ATOM_ATTR_NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
#    else
#        define ATOM_ATTR_NO_UNIQUE_ADDRESS [[no_unique_address]]
#    endif

#else
#    define ATOM_ATTR_NO_UNIQUE_ADDRESS [[no_unique_address]]

#endif

/// ------------------------------------------------------------------------------------------------
/// ATOM_CONDITIONAL_FIELD
/// ------------------------------------------------------------------------------------------------
#define ATOM_CONDITIONAL_FIELD(Condition, T)                                                       \
    ATOM_ATTR_NO_UNIQUE_ADDRESS                                                                    \
    ::Atom::TTI::TConditionalField<(Condition), T>
