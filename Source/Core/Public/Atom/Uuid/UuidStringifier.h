#pragma once
#include "Atom/Uuid/Uuid.h"
#include "Atom/Str/Str.h"
#include "Atom/Math.h"

namespace Atom
{
    class UuidStringifier
    {
        pub template <typename TOutput>
        requires ROutput<TOutput, Char>
        cexpr fn writeStr(const Uuid& uuid, TOutput&& out) const noex
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

        pub cexpr fn toStr(const Uuid& uuid) const noex -> Str
        {
            Str out;
            writeStr(uuid, out);
            return out;
        }
    };
}