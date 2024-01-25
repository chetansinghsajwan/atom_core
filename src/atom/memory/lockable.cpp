export module atom.core:lockable;
import :core;

// clang-format off

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// requirements for lockable type.
    /// --------------------------------------------------------------------------------------------
    export template <typename lockable_type>
    concept rlockable = requires(lockable_type lock)
    {
        { lock.lock() } -> rsame_as<void>;
        { lock.try_lock() } -> rsame_as<bool>;
        { lock.unlock() } -> rsame_as<void>;
    };
}

// clang-format on
