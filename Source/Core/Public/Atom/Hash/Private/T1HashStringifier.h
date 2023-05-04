#pragma once
#include "Atom/Hash/Private/T1Hash.h"
#include "Atom/String/String.h"
#include "Atom/String/StringConverter.h"

namespace Atom::Private
{
    template <typename T1Hash>
    struct T1HashStringifier
    {
        constexpr String ToString(const T1Hash& hash) noexcept
        {
            String str;
            WriteString(hash, str);
            return str;
        };

        constexpr void WriteString(const T1Hash& hash, ROutputWriter<Char> auto&& out) noexcept
        {
            for (byte b : hash.bytes)
            {
                Math::HexToChar(b, out);
            }
        }
    };
}

namespace Atom
{
    template <typename T1Hash>
    requires RDefaultConstructible<Private::T1HashStringifier<T1Hash>>
    struct StringConverter<T1Hash>
    {
        constexpr String Convert(const T1Hash& hash) noexcept
        {
            return Private::T1HashStringifier<T1Hash>()
                .ToString(hash);
        }

        constexpr void Convert(const T1Hash& hash, ROutputWriter<Char> auto&& out) noexcept
        {
            return Private::T1HashStringifier<T1Hash>()
                .WriteString(hash, out);
        }
    };
}