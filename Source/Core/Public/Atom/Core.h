#pragma once
#include <iostream>

//// -----------------------------------------------------------------------------------------------
//// Documentation Order
//// -----------------------------------------------------------------------------------------------

/// ------------------------------------------------------------------------------------------------
/// @BRIEF
/// 
/// @TPARAM
/// @PARAM[IN]
/// @PARAM[OUT]
/// 
/// @THROWS
/// 
/// @EXCEPTION SAFETY [NOEXCEPT, STRONG, WEAK, NONE]
/// 
/// @THREAD_SAFETY [SAFE, CONDITIONAL, NONE]
/// 
/// @TIME_COMPLEXITY
/// 
/// @EXAMPLES
/// 
/// @TODO
/// ------------------------------------------------------------------------------------------------

//// -----------------------------------------------------------------------------------------------
//// ATOM API
//// -----------------------------------------------------------------------------------------------

#if defined(ATOM_PLATFORM_WIN)
    #ifdef ATOM_EXPORT
        #define ATOM_API __declspec(dllexport)
    #else
        #define ATOM_API __declspec(dllimport)
    #endif

#elif defined(ATOM_PLATFORM_LINUX)
    #ifdef ATOM_EXPORT
        #define ATOM_API __attribute__((visibility("default")))
    #else
        #define ATOM_API
    #endif

#else
    #define ATOM_API
    #error "AtomEngine only supports Windows and Linux platform."

#endif

//// -----------------------------------------------------------------------------------------------
//// UTILS
//// -----------------------------------------------------------------------------------------------

#define ATOM_CONFIG_DEBUG
#define FWD(...) std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)
#define FORWARD(...) std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)

#define ATOM_FUNCTION __func__

namespace Atom
{
    using byte = unsigned char;
    using SizeT = std::size_t;
    using DiffT = long long;
    using NullType = std::nullptr_t;

    template <typename T>
    T&& MOVE(T& obj) noexcept
    {
        return std::move(obj);
    }

    template <typename T>
    void SWAP(T& lhs, T& rhs)
    {
        T tmp = MOVE(lhs);
        lhs = MOVE(rhs);
        rhs = MOVE(tmp);
    }
}