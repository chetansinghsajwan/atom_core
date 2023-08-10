#pragma once
#include "Atom/Core.h"
#include "Atom/Memory/Lockable.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// Locks the lock on construction and unlocks at destruction. This is done to guarantee
    /// exception safety.
    /// --------------------------------------------------------------------------------------------
    template <typename TLockable>
        requires(RLockable<TLockable>)
    class LockGuard
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// Constructor. Locks the lock.
        ///
        /// @PARAM[IN] lock Lockable to lock.
        ///
        /// @THROWS UnkownException Exception thrown by {lock.Lock()}.
        /// ----------------------------------------------------------------------------------------
        LockGuard(TLockable& lock)
            : _lock(lock)
        {
            _lock.Lock();
        }

        /// ----------------------------------------------------------------------------------------
        /// Destructor. Unlocks the lock.
        ///
        /// @THROWS UnkownException Exception thrown by {lock.Lock()}.
        /// ----------------------------------------------------------------------------------------
        ~LockGuard()
        {
            _lock.Unlock();
        }

    private:
        /// ----------------------------------------------------------------------------------------
        /// Lockable object.
        /// ----------------------------------------------------------------------------------------
        TLockable& _lock;
    };
}
