#pragma once
#include "Primitives/Byte.h"
#include "Primitives/Char.h"
#include "Primitives/Float.h"
#include "Atom/Memory/MemPtr.h"

// #include <typeinfo>
// #include <string_view>

/// ------------------------------------------------------------------------------------------------
/// # To Do
///
/// - Review these types, maybe we should remove them.
/// ------------------------------------------------------------------------------------------------
namespace Atom
{
    using NullPtr = std::nullptr_t;
    // static constexpr NullPtr nullptr;

    using NullVal = std::nullptr_t;
    constexpr NullVal nullval = NullVal();

    using NullPos = std::nullptr_t;
    constexpr NullPos nullpos = NullPos();

    using NullType = std::nullptr_t;
    using TypeInfo = std::type_info;

    using StdStringView = std::string_view;

    template <typename T>
    using Ptr = T*;

    using memptr = void*;

    template <typename T>
    class CtorParam
    {};
}
