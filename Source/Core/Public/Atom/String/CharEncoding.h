#pragma once
#include "Atom/String/UTF16CharEncoding.h"
#include "Atom/String/UTF16CharEncodingConverters.h"

namespace Atom
{
    using CharEncoding = UTF16CharEncoding;
    using Char = UTF16Char;
}

#define TEXT(lit) UTF16(lit)
