#pragma once
#include <string_view>

#include "Atom/Containers.h"

#include "Atom/String/BasicChar.h"

namespace Atom
{
    template <typename TEncoding>
    using BasicStringView = ::std::basic_string_view<BasicChar<TEncoding>>;
}