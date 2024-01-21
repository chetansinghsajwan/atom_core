#pragma once
// #include "atom/core/static_storage.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// _t1_hash output.
    /// --------------------------------------------------------------------------------------------
    template <usize in_size>
    class _t1_hash
    {
    public:
        static constexpr usize size = in_size;
        static const _t1_hash null;

    public:
        constexpr auto eq(const _t1_hash& that) const -> bool
        {
            return bytes.eq(that.bytes);
        }

    public:
        static_storage<size> bytes;
    };

    template <usize size>
    constexpr const _t1_hash<size> _t1_hash<size>::null = _t1_hash<size>{ 0 };
}
