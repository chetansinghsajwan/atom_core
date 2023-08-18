#pragma once
#include "BasicStr.h"
#include "DefaultEncoding.h"

namespace Atom
{
    using Str = BasicStr<CharEncoding>;
}

namespace std
{
    template <>
    class hash<Atom::Str>
    {
    public:
        auto operator()(const Atom::Str& str) const -> size_t
        {
            using TChar = Atom::BasicChar<Atom::CharEncoding>;
            using StdStirngView = basic_string_view<TChar>;

            return hash<StdStirngView>()(StdStirngView(str.data(), str.count()));
        }
    };
}
