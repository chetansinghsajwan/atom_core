#pragma once
#include "Atom/Core.h"
#include "Atom/TTI.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// Requirements for Lockable type.
    /// --------------------------------------------------------------------------------------------
    template <typename TLockable>
    concept RLockable = requires(TLockable lock)
    {
        { lock.Lock() } -> TTI::RSameAs<void>;
        { lock.TryLock() } -> TTI::RSameAs<bool>;
        { lock.Unlock() } -> TTI::RSameAs<void>;
    };
}