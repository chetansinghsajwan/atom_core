#pragma once
#include <string>

#include "Atom/String/Encoding.h"

namespace Atom
{
    template <typename TChar>
    using BasicString = ::std::basic_string<TChar>;

    using StringASCII = BasicString<CharASCII>;
    using StringUTF8 = BasicString<CharUTF8>;
    using StringUTF16 = BasicString<CharUTF16>;
    using StringUTF32 = BasicString<CharUTF32>;

    using String = BasicString<Char>;
}