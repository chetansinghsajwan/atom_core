#pragma once
#include "Atom/Containers/StaticArray.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// Sha1 Hash output.
    /// --------------------------------------------------------------------------------------------
    struct Sha1Hash
    {
        static const Sha1Hash Null;

        constexpr bool operator == (const Sha1Hash& other) const noexcept = default;
        constexpr bool operator != (const Sha1Hash& other) const noexcept = default;

        StaticArray<byte, 20> bytes;
    };

    constexpr const Sha1Hash Sha1Hash::Null = Sha1Hash{ 0 };
}