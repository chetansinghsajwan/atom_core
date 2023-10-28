#pragma once
#include "Atom/Core/Storage.h"

namespace Atom::Private
{
    /// --------------------------------------------------------------------------------------------
    /// T1Hash output.
    /// --------------------------------------------------------------------------------------------
    template <usize size>
    class T1Hash
    {
    public:
        static constexpr usize Size = size;
        static const T1Hash Null;

    public:
        constexpr auto operator==(const T1Hash& other) const -> bool = default;
        constexpr auto operator!=(const T1Hash& other) const -> bool = default;

    public:
        StaticStorage<size> bytes;
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
