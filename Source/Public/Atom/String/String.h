#pragma once
#include "BasicString.h"
#include "DefaultEncoding.h"

namespace Atom
{
    class String: public BasicString<CharEncoding>
    {
        using Base = BasicString<CharEncoding>;

    public:
        using Base::Base;
        using Base::operator=;
    };
}

namespace std
{
    template <>
    class hash<Atom::String>
    {
    public:
        auto operator()(const Atom::String& str) const -> size_t
        {
            using StdStirngView = basic_string_view<char>;

            const char* begin = str.toStdCharPtr();
            Atom::_usize count = str.count().val();
            return hash<StdStirngView>()(StdStirngView(begin, count));
        }
    };
}
