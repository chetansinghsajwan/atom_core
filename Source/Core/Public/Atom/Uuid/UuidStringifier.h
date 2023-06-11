#pragma once
#include "Atom/Uuid/Uuid.h"
#include "Atom/Str/Str.h"
#include "Atom/Math.h"

namespace Atom
{
    struct UuidStringifier
    {
        constexpr void WriteStr(const Uuid& uuid,
            ROutput<Char> auto&& out) const noexcept
        {
            out += Math::HexToChar(uuid.bytes[0]);
            out += Math::HexToChar(uuid.bytes[1]);
            out += Math::HexToChar(uuid.bytes[2]);
            out += Math::HexToChar(uuid.bytes[3]);
            out += TEXT('-');
            out += Math::HexToChar(uuid.bytes[4]);
            out += Math::HexToChar(uuid.bytes[5]);
            out += TEXT('-');
            out += Math::HexToChar(uuid.bytes[6]);
            out += Math::HexToChar(uuid.bytes[7]);
            out += TEXT('-');
            out += Math::HexToChar(uuid.bytes[8]);
            out += Math::HexToChar(uuid.bytes[9]);
            out += TEXT('-');
            out += Math::HexToChar(uuid.bytes[10]);
            out += Math::HexToChar(uuid.bytes[11]);
            out += Math::HexToChar(uuid.bytes[12]);
            out += Math::HexToChar(uuid.bytes[13]);
            out += Math::HexToChar(uuid.bytes[14]);
            out += Math::HexToChar(uuid.bytes[15]);
        }

        constexpr Str ToStr(const Uuid& uuid) const noexcept
        {
            Str out;
            WriteStr(uuid, out);
            return out;
        }
    };
}