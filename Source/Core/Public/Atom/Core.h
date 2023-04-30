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
//// COMPILER
//// -----------------------------------------------------------------------------------------------

#if defined(_MSC_VER)
    #define ATOM_COMPILER_MSVC
    #define ATOM_COMPILER_MSVC_VER _MSC_VER
    #define ATOM_COMPILER_MSVC_FULL_VER _MSC_FULL_VER
    #define ATOM_COMPILER_MSVC_VER_MAJOR (ATOM_COMPILER_MSVC_VER / 100)
    #define ATOM_COMPILER_MSVC_VER_MINOR (ATOM_COMPILER_MSVC_VER % 100)
    #define ATOM_COMPILER_MSVC_VER_PATCH (ATOM_COMPILER_MSVC_VER)
    #define ATOM_COMPILER_MSVC_VER_BUILD _MSC_BUILD

#endif

#if defined(__GNUC__)
    #define ATOM_COMPILER_GCC
    #define ATOM_COMPILER_GCC_VER_MAJOR __GNUC__
    #define ATOM_COMPILER_GCC_VER_MINOR __GNUC_MINOR__
    #define ATOM_COMPILER_GCC_VER_PATCH __GNUC_PATCHLEVEL__
    #define ATOM_COMPILER_GCC_VER (ATOM_COMPILER_GCC_VER_MAJOR * 100) + ATOM_COMPILER_GCC_VER_MINOR
    #define ATOM_COMPILER_GCC_FULL_VER (ATOM_COMPILER_GCC_VER * 100) + ATOM_COMPILER_GCC_VER_PATCH

#endif

#if defined(__clang__)
    #define ATOM_COMPILER_CLANG
    #define ATOM_COMPILER_CLANG_VER_MAJOR __clang_major__
    #define ATOM_COMPILER_CLANG_VER_MINOR __clang_minor__
    #define ATOM_COMPILER_CLANG_VER_PATCH __clang_patchlevel__
    #define ATOM_COMPILER_CLANG_FULL_VER __clang_version__

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

#define ATOM_IS_CONFIG_DEBUG true
#define ATOM_IF_DEBUG if constexpr (ATOM_IS_CONFIG_DEBUG)

#define FORWARD(...) std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)

#define ATOM_FUNCTION __func__

#define declval(T) ::std::declval<T>()

namespace Atom
{
    using byte = unsigned char;

    using uchar = unsigned char;
    using char8 = char8_t;
    using char16 = char16_t;
    using char32 = char32_t;

    using int8 = int8_t;
    using int16 = int16_t;
    using int32 = int32_t;
    using int64 = int64_t;

    using uint8 = uint8_t;
    using uint16 = uint16_t;
    using uint32 = uint32_t;
    using uint64 = uint64_t;
    using uint = unsigned int;

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
    constexpr T& MOVE(T& lhs, T& rhs) noexcept
    {
        lhs = std::move(rhs);
        return lhs;
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