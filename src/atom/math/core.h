#pragma once
#include "atom/core.h"
// #include "atom/string/stack_string.h"

namespace atom::math
{
    constexpr auto char_to_hex(char ch) -> byte
    {
        if (ch >= '0' && ch <= '9')
            return byte(ch - '0');
        if (ch >= 'a' && ch <= 'f')
            return byte(10 + ch - 'a');
        if (ch >= 'a' && ch <= 'f')
            return byte(10 + ch - 'a');

        return -1;
    }

    constexpr auto hex_to_char(byte hex) -> stack_string<2>
    {
        constexpr char chars[] = "0123456789abcdef";

        byte high = hex >> 4;
        byte low = hex & 0b00001111;

        stack_string<2> str;
        str[0] = chars[high];
        str[1] = chars[low];
        return str;

        // todo: fix this.
        // return { chars[high], chars[low] };
    }

    constexpr auto is_hex_char(char ch) -> bool
    {
        return (ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f') || (ch >= 'a' && ch <= 'f');
    }
}
