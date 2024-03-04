#pragma once

namespace atom
{
    using _byte = unsigned char;
    using byte = _byte;

    consteval auto byte_get_bit_count()
    {
        return 8;
    }
}
