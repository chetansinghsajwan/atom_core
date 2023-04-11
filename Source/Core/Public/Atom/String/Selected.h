#pragma once
#include "Atom/String/UTF16CharEncoding.h"
#include "Atom/String/UTF16CharEncodingConverters.h"

namespace Atom
{
    using CharEncoding = UTF16CharEncoding;

    using Char = UTF16Char;

    using String = UTF16String;
    using StringWrapper = BasicStringWrapper<UTF16CharEncoding>;
    using StringViewWrapper = BasicStringViewWrapper<UTF16CharEncoding>;

    using StringView = UTF16StringView;
}

#define TEXT(lit) UTF16(lit)
