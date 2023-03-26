#pragma once
#include <iostream>

//// -----------------------------------------------------------------------------------------------
//// Platform
//// -----------------------------------------------------------------------------------------------

#if defined(_WIN32)
    #define ATOM_PLATFORM_WINDOWS
#endif

#if defined(__unix__)
    #define ATOM_PLATFORM_LINUX
#endif

//// -----------------------------------------------------------------------------------------------
//// ATOM API
//// -----------------------------------------------------------------------------------------------

#if defined(ATOM_PLATFORM_WINDOWS)
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
    #error "AtomEngine only supports Windows and Linux platform for now."

#endif

//// -----------------------------------------------------------------------------------------------
//// UTILS
//// -----------------------------------------------------------------------------------------------

#define ATOM_CONFIG_DEBUG true

#if defined(ATOM_CONFIG_DEBUG)
    #define ATOM_DEBUG_IF if constexpr (true)
#else
    #define ATOM_DEBUG_IF if constexpr (false)
#endif

#define FORWARD(...) std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)

#define ATOM_FUNCTION __func__

#define declval(T) ::std::declval<T>()

namespace Atom
{
    using byte = unsigned char;
    using usize = std::size_t;
    using isize = std::ptrdiff_t;

    using NullType = std::nullptr_t;
    using TypeInfo = std::type_info;

    template <typename T>
    constexpr T&& MOVE(T& obj) noexcept
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

#include "Atom/Core/Requirements.h"