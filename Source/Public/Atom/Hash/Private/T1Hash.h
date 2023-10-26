#pragma once
#include "Atom/Containers/StaticArray.h"

namespace Atom::Private
{
    /// --------------------------------------------------------------------------------------------
    /// T1Hash output.
    /// --------------------------------------------------------------------------------------------
    template <usize Size>
    class T1Hash
    {
    public:
        static constexpr usize _Size = Size;
        static const T1Hash Null;

    public:
        constexpr auto operator==(const T1Hash& other) const -> bool = default;
        constexpr auto operator!=(const T1Hash& other) const -> bool = default;

    public:
        StaticArray<byte, Size> bytes;
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
