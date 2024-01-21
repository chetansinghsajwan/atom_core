#pragma once
// #include "atom/core.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// requirements for lockable type.
    /// --------------------------------------------------------------------------------------------
    template <typename tlockable>
    concept rlockable = requires(tlockable lock) {
        {
            lock.lock()
        } -> rsame_as<void>;
        {
            lock.try_lock()
        } -> rsame_as<bool>;
        {
            lock.unlock()
        } -> rsame_as<void>;
    };
}
