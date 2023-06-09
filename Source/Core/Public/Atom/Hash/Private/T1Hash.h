#pragma once
#include "Atom/Containers/StaArr.h"

namespace Atom::Private
{
    /// T1Hash output.
    /// --------------------------------------------------------------------------------------------
    template <usize size>
    struct T1Hash
    {
        static constexpr usize _Size = size;

        static const T1Hash Null;

        constexpr bool operator == (const T1Hash& other) const noexcept = default;
        constexpr bool operator != (const T1Hash& other) const noexcept = default;

        StaArr<byte, size> bytes;
    };

    // FIX: Compilation error in MSVC, checkout /Zc:externConstexpr
    template <usize size>
#ifdef ATOM_COMP_MSVC
    inline
#else
    constexpr
#endif
    const T1Hash<size> T1Hash<size>::Null = T1Hash<size>{ 0 };
}