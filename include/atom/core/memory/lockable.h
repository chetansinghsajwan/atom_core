#pragma once
#include "atom/core/core.h"

// clang-format off

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// requirements for lockable type.
    /// --------------------------------------------------------------------------------------------
    template <typename lockable_t>
    concept is_lockable = requires(lockable_t lock)
    {
        { lock.lock() } -> is_void;
        { lock.try_lock() } -> is_same_as<bool>;
        { lock.unlock() } -> is_void;
    };
}

// clang-format on
