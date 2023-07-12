#pragma once
#include <iostream>
#include <cstdint>

#include "LangExtensions.h"

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
}

namespace Atom
{
    using NullPtr = std::nullptr_t;
    // static constexpr NullPtr nullptr;
}

namespace Atom
{
    using NullVal = std::nullptr_t;
    static constexpr NullVal nullval = NullVal();
}

namespace Atom
{
    using NullPos = std::nullptr_t;
    static constexpr NullPos nullpos = NullPos();
}

namespace Atom
{
    using NullType = std::nullptr_t;
    using TypeInfo = std::type_info;

    template <typename T>
    constexpr T&& MOVE(T& obj) noex
    {
        return std::move(obj);
    }

    template <typename T>
    constexpr T& MOVE(T& lhs, T& rhs) noex
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

#define ATOM_IS_CONFIG_DEBUG true
#define ATOM_IF_DEBUG if constexpr (ATOM_IS_CONFIG_DEBUG)

#define FORWARD(...) std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)

#define ATOM_FUNCTION __func__
#define ATOM_ATTR_NORETURN [[noreturn]]

#define declval(T) ::std::declval<T>()
