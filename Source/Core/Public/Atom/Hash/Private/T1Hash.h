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
        pub static constexpr usize _Size = Size;

        pub static const T1Hash Null;

        pub constexpr fn op==(const T1Hash& other) const -> bool = default;
        pub constexpr fn op!=(const T1Hash& other) const -> bool = default;

        pub StaticArr<byte, Size> bytes;
    };

    // FIX: Compilation error in MSVC, checkout /Zc:externConstexpr
    template <usize Size>
#ifdef ATOM_COMP_MSVC
    inline
#else
    constexpr
#endif
    const T1Hash<Size> T1Hash<Size>::Null = T1Hash<Size>{ 0 };
}