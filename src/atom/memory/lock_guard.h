#pragma once
// #include "atom/core.h"
#include "atom/memory/lockable.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// locks the lock on construction and unlocks at destruction. this_type is done to guarantee
    /// exception safety.
    /// --------------------------------------------------------------------------------------------
    template <typename tlockable>
        requires(rlockable<tlockable>)
    class lock_guard
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// constructor. locks the lock.
        ///
        /// @param[in] lock lockable to lock.
        ///
        /// @throws unkown_exception exception thrown by {lock.lock()}.
        /// ----------------------------------------------------------------------------------------
        lock_guard(tlockable& lock)
            : _lock(lock)
        {
            _lock.lock();
        }

        /// ----------------------------------------------------------------------------------------
        /// destructor. unlocks the lock.
        ///
        /// @throws unkown_exception exception thrown by {lock.lock()}.
        /// ----------------------------------------------------------------------------------------
        ~lock_guard()
        {
            _lock.unlock();
        }

    private:
        /// ----------------------------------------------------------------------------------------
        /// lockable object.
        /// ----------------------------------------------------------------------------------------
        tlockable& _lock;
    };
}
