#pragma once
#include "Atom/Hash/Private/T1Hash.h"
#include "Atom/Str/Str.h"
#include "Atom/Str/StrConverter.h"

namespace Atom::Private
{
    template <typename T1Hash>
    struct T1HashStringifier
    {
        constexpr Str ToStr(const T1Hash& hash) noex
        {
            Str str;
            WriteStr(hash, str);
            return str;
        };

        constexpr void WriteStr(const T1Hash& hash, ROutput<Char> auto&& out) noex
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
    requires RDefaultConstructible<Private::T1HashStringifier<T1Hash>>
    struct StrConverter<T1Hash>
    {
        constexpr Str Convert(const T1Hash& hash) noex
        {
            return Private::T1HashStringifier<T1Hash>()
                .ToStr(hash);
        }

        constexpr void Convert(const T1Hash& hash, ROutput<Char> auto&& out) noex
        {
            return Private::T1HashStringifier<T1Hash>()
                .WriteStr(hash, out);
        }
    };
}