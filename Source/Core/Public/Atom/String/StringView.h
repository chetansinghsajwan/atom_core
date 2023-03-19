#pragma once
#include <string_view>

#include "Atom/String/Encoding.h"

namespace Atom
{
    template <typename TChar>
    using BasicStringView = ::std::basic_string_view<TChar>;

    using StringViewASCII = BasicStringView<CharASCII>;
    using StringViewUTF8 = BasicStringView<CharUTF8>;
    using StringViewUTF16 = BasicStringView<CharUTF16>;
    using StringViewUTF32 = BasicStringView<CharUTF32>;

    using StringView = BasicStringView<Char>;
}