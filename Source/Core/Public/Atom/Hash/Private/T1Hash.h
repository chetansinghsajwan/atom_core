#pragma once
#include "Atom/Containers/StaArr.h"

namespace Atom::Private
{
    /// --------------------------------------------------------------------------------------------
    /// T1Hash output.
    /// --------------------------------------------------------------------------------------------
    template <usize Size>
    struct T1Hash
    {
        static cexpr usize _Size = Size;

        static const T1Hash Null;

        cexpr fn operator == (const T1Hash& other) const noex -> bool = default;
        cexpr fn operator != (const T1Hash& other) const noex -> bool = default;

        StaArr<byte, Size> bytes;
    };

    // FIX: Compilation error in MSVC, checkout /Zc:externConstexpr
    template <usize Size>
#ifdef ATOM_COMP_MSVC
    inline
#else
    cexpr
#endif
    const T1Hash<Size> T1Hash<Size>::Null = T1Hash<Size>{ 0 };
}