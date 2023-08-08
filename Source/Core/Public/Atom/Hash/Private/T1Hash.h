#pragma once
#include "Atom/Containers/StaticArr.h"

namespace Atom::Private
{
    /// --------------------------------------------------------------------------------------------
    /// T1Hash output.
    /// --------------------------------------------------------------------------------------------
    template <usize Size>
    class T1Hash
    {
        pub static cexpr usize _Size = Size;

        pub static const T1Hash Null;

        pub cexpr fn op==(const T1Hash& other) const noex -> bool = default;
        pub cexpr fn op!=(const T1Hash& other) const noex -> bool = default;

        pub StaticArr<byte, Size> bytes;
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