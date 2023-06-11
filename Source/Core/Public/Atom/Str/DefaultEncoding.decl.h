#pragma once
#include "Atom/Text/Utf16Encoding.decl.h"

#define ATOM_TEXT(text) ATOM_TEXT_UTF16(text)
#define TEXT(text) ATOM_TEXT_UTF16(text)

namespace Atom
{
    using CharEncoding = Utf16Encoding;
    using Char = Utf16Char;
    using Rune = Utf16Rune;
}