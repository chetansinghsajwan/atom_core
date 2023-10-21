#pragma once
#include "BasicStr.h"
#include "DefaultEncoding.h"

namespace Atom
{
    class Str: public BasicStr<CharEncoding>
    {
        using Base = BasicStr<CharEncoding>;

    public:
        using Base::Base;
        using Base::operator=;
    };
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
