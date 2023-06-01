#pragma once
#include "BasicString.decl.h"
#include "DefaultEncoding.decl.h"

namespace Atom
{
    using String = BasicString<CharEncoding>;
}

namespace std
{
    template < >
    struct hash<Atom::String>
    {
        size_t operator ()(const Atom::String& str) const noexcept
        {
            using TChar = Atom::BasicChar<Atom::CharEncoding>;
            using StdStirngView = basic_string_view<TChar>;

            return hash<StdStirngView>()(StdStirngView(str.Data(), str.Count()));
        }
    };
}