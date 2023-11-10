#pragma once
#include "Atom/Core.h"
#include "Atom/String/StackString.h"

namespace Atom::Math
{
    constexpr auto CharToHex(Char ch) -> byte
    {
        if (ch >= '0' && ch <= '9')
            return byte(ch - '0');
        if (ch >= 'a' && ch <= 'f')
            return byte(10 + ch - 'a');
        if (ch >= 'A' && ch <= 'F')
            return byte(10 + ch - 'A');

        return -1;
    }

    constexpr auto HexToChar(byte hex) -> StackString<2>
    {
        constexpr char chars[] = "0123456789abcdef";

        byte high = hex >> 4;
        byte low = hex & 0b00001111;

        StackString<2> str;
        str[0] = chars[high];
        str[1] = chars[low];
        return str;

        // TODO: fix this.
        // return { chars[high], chars[low] };
    }

    constexpr auto IsHexChar(Char ch) -> bool
    {
        return (ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F');
    }
}
