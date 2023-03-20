#pragma once
#include "Atom/Core.h"

namespace Atom
{
    using CharASCII = char;
    using CharUTF8 = char8_t;
    using CharUTF16 = char16_t;
    using CharUTF32 = char32_t;

    using Char = CharUTF16;
}

#define TEXT(txt) u##txt