#pragma once
#include <string>

#include "Atom/String/Encoding.h"

#include "Atom/Containers.h"

namespace Atom
{
    template <typename TChar>
    using BasicString = ::std::basic_string<TChar>;

    using StringASCII = BasicString<CharASCII>;
    using StringUTF8 = BasicString<CharUTF8>;
    using StringUTF16 = BasicString<CharUTF16>;
    using StringUTF32 = BasicString<CharUTF32>;

    using String = BasicString<Char>;

    struct StringWrapper
    {
        constexpr StringWrapper(String& in_str) noexcept:
            str{ in_str } { }

        constexpr void InsertBack(Char ch) noexcept
        {
            str.push_back(ch);
        }

        constexpr void InsertBack(const RConstIterable<Char> auto& chars) noexcept
        {
            str.insert(str.end(), chars.Begin(), chars.End());
        }

        String& str;
    };

    static_assert(RBackInsertable<StringWrapper, Char>,
        "StringWrapper does not satisfy RBackInsertable requirements.");
}