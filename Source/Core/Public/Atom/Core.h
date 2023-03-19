#pragma once
#include <iostream>

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

#define FWD(...) std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)
#define FORWARD(...) std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)

#define ATOM_FUNCTION __func__

namespace Atom
{
    using byte = unsigned char;
    using SizeT = std::size_t;
    using DiffT = long long;
    using NullType = std::nullptr_t;
    using TypeInfo = std::type_info;

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