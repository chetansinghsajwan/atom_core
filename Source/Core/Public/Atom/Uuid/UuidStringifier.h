#pragma once
#include "Atom/Math.h"
#include "Atom/Str/Str.h"
#include "Atom/Uuid/Uuid.h"

namespace Atom
{
    class UuidStringifier
    {
    public:
        template <typename TOutput>
        constexpr auto writeStr(const Uuid& uuid, TOutput&& out) const
            requires(ROutput<TOutput, Char>)
        {
            out += Math::HexToChar(uuid.bytes[0]);
            out += Math::HexToChar(uuid.bytes[1]);
            out += Math::HexToChar(uuid.bytes[2]);
            out += Math::HexToChar(uuid.bytes[3]);
            out += '-';
            out += Math::HexToChar(uuid.bytes[4]);
            out += Math::HexToChar(uuid.bytes[5]);
            out += '-';
            out += Math::HexToChar(uuid.bytes[6]);
            out += Math::HexToChar(uuid.bytes[7]);
            out += '-';
            out += Math::HexToChar(uuid.bytes[8]);
            out += Math::HexToChar(uuid.bytes[9]);
            out += '-';
            out += Math::HexToChar(uuid.bytes[10]);
            out += Math::HexToChar(uuid.bytes[11]);
            out += Math::HexToChar(uuid.bytes[12]);
            out += Math::HexToChar(uuid.bytes[13]);
            out += Math::HexToChar(uuid.bytes[14]);
            out += Math::HexToChar(uuid.bytes[15]);
        }

        constexpr auto toStr(const Uuid& uuid) const -> Str
        {
            Str out;
            writeStr(uuid, out);
            return out;
        }
    };
}
