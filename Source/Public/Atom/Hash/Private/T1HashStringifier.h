#pragma once
#include "Atom/Hash/Private/T1Hash.h"
#include "Atom/String/String.h"
#include "Atom/String/StringConverter.h"

namespace Atom::Private
{
    template <typename T1Hash>
    class T1HashStringifier
    {
    public:
        constexpr auto ToString(const T1Hash& hash) -> String
        {
            String str;
            WriteString(hash, str);
            return str;
        };

        template <typename TOut>
        constexpr auto WriteString(const T1Hash& hash, TOut&& out)
            requires(ROutput<TOut, Char>)
        {
            // for (byte b : hash.bytes)
            // {
            //     StackString<2> chars = Math::HexToChar(b);
            //     out += chars[0];
            //     out += chars[1];
            // }
        }
    };
}

namespace Atom
{
    template <typename T1Hash>
        requires(RDefaultConstructible<Private::T1HashStringifier<T1Hash>>)
    class StringConverter<T1Hash>
    {
    public:
        constexpr auto Convert(const T1Hash& hash) -> String
        {
            return Private::T1HashStringifier<T1Hash>().ToString(hash);
        }

        constexpr auto Convert(const T1Hash& hash, ROutput<Char> auto&& out)
        {
            return Private::T1HashStringifier<T1Hash>().WriteString(hash, out);
        }
    };
}
