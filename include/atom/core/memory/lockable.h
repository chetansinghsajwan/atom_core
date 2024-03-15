#pragma once
#include "atom/core/core.h"

// clang-format off

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// requirements for lockable type.
    /// --------------------------------------------------------------------------------------------
    template <typename lockable_t>
    concept rlockable = requires(lockable_t lock)
    {
        { lock.lock() } -> rsame_as<void>;
        { lock.try_lock() } -> rsame_as<bool>;
        { lock.unlock() } -> rsame_as<void>;
    };
}

// clang-format on
