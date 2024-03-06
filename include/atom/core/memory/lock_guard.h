#pragma once
#include "atom/core/memory/lockable.h"

namespace atom
{
    /// --------------------------------------------------------------------------------------------
    /// locks the lock on construction and unlocks at destruction. this_type is done to guarantee
    /// exception safety.
    /// --------------------------------------------------------------------------------------------
    template <typename lockable_type>
    class lock_guard
    {
        ATOM_STATIC_ASSERTS(rlockable<lockable_type>);

    public:
        /// ----------------------------------------------------------------------------------------
        /// constructor. locks the lock.
        ///
        /// @param[in] lock lockable to lock.
        ///
        /// @throws unkown_exception exception thrown by {lock.lock()}.
        /// ----------------------------------------------------------------------------------------
        lock_guard(lockable_type& lock)
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
        lockable_type& _lock;
    };
}
