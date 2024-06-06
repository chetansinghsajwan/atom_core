export module atom.core:lockable;

import std;

// clang-format off

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// requirements for lockable type.
    /// --------------------------------------------------------------------------------------------
    export template <typename lockable_type>
    concept is_lockable = requires(lockable_type lock)
    {
        { lock.lock() } -> std::same_as<void>;
        { lock.try_lock() } -> std::same_as<bool>;
        { lock.unlock() } -> std::same_as<void>;
    };
}

// clang-format on
