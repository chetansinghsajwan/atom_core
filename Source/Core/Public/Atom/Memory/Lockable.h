#pragma once
#include "Atom/Core.h"
#include "Atom/TTI.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// Requirements for Lockable type.
    /// --------------------------------------------------------------------------------------------
    template <typename LockableType>
    concept LockableRequirements = requires(LockableType lock)
    {
        { lock.Lock() } -> TTI::SameAs<void>;
        { lock.TryLock() } -> TTI::SameAs<bool>;
        { lock.Unlock() } -> TTI::SameAs<void>;
    };

    /// --------------------------------------------------------------------------------------------
    /// Alias for LockableRequirements to use inline with auto.
    /// --------------------------------------------------------------------------------------------
    template <typename LockableType>
    concept Lockable = LockableRequirements<LockableType>;
}