#pragma once
#include "Atom/Hash/Private/T1Hash.h"
#include "Atom/Str/Str.h"
#include "Atom/Str/StrConverter.h"

namespace Atom::Private
{
    template <typename T1Hash>
    class T1HashStringifier
    {
    public:
        constexpr auto ToStr(const T1Hash& hash) -> Str
        {
            Str str;
            WriteStr(hash, str);
            return str;
        };

        constexpr auto WriteStr(const T1Hash& hash, ROutput<Char> auto&& out)
        {
            for (byte b : hash.bytes)
            {
                StaticStr<2> chars = Math::HexToChar(b);
                out += chars[0];
                out += chars[1];
            }
        }
    };
}

namespace Atom
{
    template <typename T1Hash>
        requires(RDefaultConstructible<Private::T1HashStringifier<T1Hash>>)
    class StrConverter<T1Hash>
    {
    public:
        constexpr auto Convert(const T1Hash& hash) -> Str
        {
            return Private::T1HashStringifier<T1Hash>().ToStr(hash);
        }

        constexpr auto Convert(const T1Hash& hash, ROutput<Char> auto&& out)
        {
            return Private::T1HashStringifier<T1Hash>().WriteStr(hash, out);
        }
    };
}
