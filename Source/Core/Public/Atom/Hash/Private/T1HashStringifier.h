#pragma once
#include "Atom/Hash/Private/T1Hash.h"
#include "Atom/Str/Str.h"
#include "Atom/Str/StrConverter.h"

namespace Atom::Private
{
    template <tname T1Hash>
    struct T1HashStringifier
    {
        cexpr fn ToStr(const T1Hash& hash) noex -> Str
        {
            Str str;
            WriteStr(hash, str);
            return str;
        };

        cexpr fn WriteStr(const T1Hash& hash, ROutput<Char> auto&& out) noex
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
    template <tname T1Hash>
    requires RDefaultConstructible<Private::T1HashStringifier<T1Hash>>
    struct StrConverter<T1Hash>
    {
        cexpr fn Convert(const T1Hash& hash) noex -> Str
        {
            return Private::T1HashStringifier<T1Hash>()
                .ToStr(hash);
        }

        cexpr fn Convert(const T1Hash& hash, ROutput<Char> auto&& out) noex
        {
            return Private::T1HashStringifier<T1Hash>()
                .WriteStr(hash, out);
        }
    };
}