#pragma once
#include "Atom/Containers.h"

namespace Atom
{
    enum struct EUuidVersion : byte
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

    enum struct EUuidVariant : byte
    {
        NCS,
        RFC,
        Microsoft,
        Reserved
    };

    struct Uuid
    {
        static const Uuid Null;

        constexpr bool operator == (const Uuid& other) const noex = default;
        constexpr bool operator != (const Uuid& other) const noex = default;

        StaArr<byte, 16> bytes;
    };

    constexpr inline const Uuid Uuid::Null = Uuid{ 0 };
}