#pragma once
// #include "atom/core/core.h"

// clang-format off

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// requirements for lockable type.
    /// --------------------------------------------------------------------------------------------
    template <typename lockable_t>
    concept is_lockable = requires(lockable_t lock)
    {
        { lock.lock() } -> std::same_as<void>;
        { lock.try_lock() } -> std::same_as<bool>;
        { lock.unlock() } -> std::same_as<void>;
    };
}

// clang-format on
