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

    using i8 = int8_t;
    using i16 = int16_t;
    using i32 = int32_t;
    using i64 = int64_t;

    using u8 = uint8_t;
    using u16 = uint16_t;
    using u32 = uint32_t;
    using u64 = uint64_t;

    using f32 = float;
    using f64 = double;
    using f128 = long double;

    using isize = std::ptrdiff_t;
    using usize = std::size_t;
}

namespace Atom
{
    using NullPtr = std::nullptr_t;
    // static cexpr NullPtr nullptr;

    using NullVal = std::nullptr_t;
    static cexpr NullVal nullval = NullVal();

    using NullPos = std::nullptr_t;
    static cexpr NullPos nullpos = NullPos();

    using NullType = std::nullptr_t;
    using TypeInfo = std::type_info;
}