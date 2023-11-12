#pragma once
#include "Atom/Core/StaticStorage.h"

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
        constexpr auto eq(const T1Hash& that) const -> bool
        {
            return bytes.eq(that.bytes);
        }

        constexpr auto ne(const T1Hash& that) const -> bool
        {
            return bytes.ne(that.bytes);
        }

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
