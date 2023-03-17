#pragma once
#include "Atom/Containers.h"

namespace Atom
{
    enum struct EUuidVersion: byte
    {
        None = 0,

        V1 = 1,
        V2 = 2,
        V3 = 3,
        V4 = 4,
        V5 = 5,

        TimeBased = V1,
        DceSecurity = V2,
        NameBasedMD5 = V3,
        RandomNumberBased = V4,
        NameBasedSHA1 = V5
    };

    enum struct EUuidVariant: byte
    {
        NCS,
        RFC,
        Microsoft,
        Reserved
    };

    struct Uuid
    {
        StaticArray<byte, 16> bytes;
    };

    constexpr bool operator == (const Uuid& in_lhs, const Uuid& in_rhs) noexcept
    {
        return in_lhs.bytes == in_rhs.bytes;
    }

    constexpr bool operator != (const Uuid& in_lhs, const Uuid& in_rhs) noexcept
    {
        return in_lhs.bytes != in_rhs.bytes;
    }
}