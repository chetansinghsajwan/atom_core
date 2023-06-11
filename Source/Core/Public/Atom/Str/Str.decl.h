#pragma once
#include "BasicStr.decl.h"
#include "DefaultEncoding.decl.h"

namespace Atom
{
    using Str = BasicStr<CharEncoding>;
}

namespace std
{
    template < >
    struct hash<Atom::Str>
    {
        size_t operator ()(const Atom::Str& str) const noexcept
        {
            using TChar = Atom::BasicChar<Atom::CharEncoding>;
            using StdStirngView = basic_string_view<TChar>;

            return hash<StdStirngView>()(StdStirngView(str.Data(), str.Count()));
        }
    };
}