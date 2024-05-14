#pragma once
#include "atom/core/memory/lockable.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// locks the lock on construction and unlocks at destruction. this_t is done to guarantee
    /// exception safety.
    /// --------------------------------------------------------------------------------------------
    template <typename lockable_t>
    class lock_guard
    {
        static_assert(is_lockable<lockable_t>);

    public:
        /// ----------------------------------------------------------------------------------------
        /// constructor. locks the lock.
        ///
        /// @param[in] lock lockable to lock.
        ///
        /// @throws unkown_exception exception thrown by {lock.lock()}.
        /// ----------------------------------------------------------------------------------------
        lock_guard(lockable_t& lock)
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
        lockable_t& _lock;
    };
}
