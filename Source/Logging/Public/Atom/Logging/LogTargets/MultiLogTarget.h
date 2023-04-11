#pragma once
#include "Atom/Memory.h"
#include "Atom/Containers.h"
#include "Atom/Exceptions.h"
#include "Atom/Logging/LogTarget.h"

namespace Atom::Logging::Internal
{
    /// --------------------------------------------------------------------------------------------
    /// --- DOC_TEMPLATE 
    /// --------------------------------------------------------------------------------------------
    /// MultiLogTargetTemplate is used to store multiple LogTarget objects.
    /// 
    /// @TPARAM TLockable Type used to ensure thread safety.
    /// 
    /// @REQUIRES TLockable RLockable<TLockable>.
    /// 
    /// @THREAD_SAFETY SAFE
    /// --------------------------------------------------------------------------------------------
    template <RLockable TLockable>
    class MultiLogTargetTemplate: public LogTarget
    {
        using ThisT = MultiLogTargetTemplate;
        using Container = DynamicArray<LogTargetPtr>;
        using TConstIterator = typename Container::const_iterator;

    public:
        /// ----------------------------------------------------------------------------------------
        /// Default constructor.
        /// ----------------------------------------------------------------------------------------
        MultiLogTargetTemplate() noexcept { }

        /// ----------------------------------------------------------------------------------------
        /// Constructs and adds LogTarget object.
        /// 
        /// @PARAM[IN] target LogTarget object to add.
        ///     If {target} is null, this doesn't adds it.
        /// 
        /// @EXCEPTION_SAFETY @COPY_FROM _AddTarget(LogTargetPtr target).
        /// 
        /// @TIME_COMPLEXITY @COPY_FROM _AddTarget(LogTargetPtr target).
        /// ----------------------------------------------------------------------------------------
        MultiLogTargetTemplate(LogTargetPtr target)
        {
            _AddTarget(target);
        }

        /// ----------------------------------------------------------------------------------------
        /// Constructs and adds LogTarget objects.
        /// 
        /// @PARAM[IN] target InitializerList of LogTarget objects to add.
        ///     If {targets} contains null objects, this doesn't adds them.
        /// 
        /// @EXCEPTION_SAFETY @COPY_FROM _AddTargets(RConstIterator<LogTargetPtr> auto begin,
        ///     RConstIterator<LogTargetPtr> auto end)
        /// 
        /// @TIME_COMPLEXITY @COPY_FROM _AddTargets(RConstIterator<LogTargetPtr> auto begin,
        ///     RConstIterator<LogTargetPtr> auto end)
        /// ----------------------------------------------------------------------------------------
        MultiLogTargetTemplate(InitializerList<LogTargetPtr> targets)
        {
            _AddTargets(targets.begin(), targets.end());
        }

        /// ----------------------------------------------------------------------------------------
        /// Constructs with LogTarget objects.
        /// 
        /// @PARAM[IN] target RConstIterable of LogTarget objects to add.
        ///     If {targets} contains null objects, this doesn't adds them.
        /// 
        /// @EXCEPTION_SAFETY @COPY_FROM _AddTargets(RConstIterator<LogTargetPtr> auto begin,
        ///     RConstIterator<LogTargetPtr> auto end)
        /// 
        /// @TIME_COMPLEXITY @COPY_FROM _AddTargets(RConstIterator<LogTargetPtr> auto begin,
        ///     RConstIterator<LogTargetPtr> auto end)
        /// ----------------------------------------------------------------------------------------
        MultiLogTargetTemplate(const RConstIterable<LogTargetPtr> auto& targets)
        {
            _AddTargets(targets.begin(), targets.end());
        }

        /// ----------------------------------------------------------------------------------------
        /// Constructs with LogTarget objects.
        /// 
        /// @PARAM[IN] begin, end RConstIterator to beginning and end of range to add.
        ///     If range {[begin, end]} contains null objects, this doesn't adds them.
        /// 
        /// @EXCEPTION_SAFETY @COPY_FROM _AddTargets(RConstIterator<LogTargetPtr> auto begin,
        ///     RConstIterator<LogTargetPtr> auto end)
        /// 
        /// @TIME_COMPLEXITY @COPY_FROM _AddTargets(RConstIterator<LogTargetPtr> auto begin,
        ///     RConstIterator<LogTargetPtr> auto end)
        /// ----------------------------------------------------------------------------------------
        MultiLogTargetTemplate(RConstIterator<LogTargetPtr> auto begin,
            RConstIterator<LogTargetPtr> auto end)
        {
            _AddTargets(begin, end);
        }

    //// -------------------------------------------------------------------------------------------
    //// LogTarget Interface
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// Writes to all stored LogTarget objects.
        /// 
        /// @EXCEPTION_SAFETY STRONG
        /// ----------------------------------------------------------------------------------------
        virtual void Write(const LogMsg& logMsg) override final
        {
            LockGuard guard(_lock);
            for (auto& target : _targets)
            {
                target->Write(logMsg);
            }
        }

        /// ----------------------------------------------------------------------------------------
        /// Flushes all stored LogTarget objects.
        /// 
        /// @EXCEPTION_SAFETY STRONG
        /// ----------------------------------------------------------------------------------------
        virtual void Flush() override final
        {
            LockGuard guard(_lock);
            for (auto& target : _targets)
            {
                target->Flush();
            }
        }

    //// -------------------------------------------------------------------------------------------
    //// LogTarget manipulation functions
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// Adds LogTarget object.
        /// 
        /// @PARAM[IN] target LogTarget object to add.
        ///     If {target} is null, this doesn't adds it.
        /// @RETURNS {true} if added, else {false}.
        /// 
        /// @EXCEPTION_SAFETY @COPY_FROM _AddTarget(LogTargetPtr target).
        /// 
        /// @TIME_COMPLEXITY @COPY_FROM _AddTarget(LogTargetPtr target).
        /// 
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        bool AddTarget(LogTargetPtr target) noexcept
        {
            if (target == nullptr) return false;

            LockGuard guard(_lock);
            _AddTarget(target);
        }

        /// ---------------------------------------------------------------------------------------- 
        /// Adds LogTarget objects.
        /// 
        /// @PARAM[IN] targets LogTarget objects to add.
        ///     If {targets} contains null objects, this doesn't adds them.
        /// @RETURNS Count of LogTarget objects added.
        /// 
        /// @EXCEPTION_SAFETY @COPY_FROM _AddTargets(RConstIterator<LogTargetPtr> auto begin,
        ///     RConstIterator<LogTargetPtr> auto end)
        /// 
        /// @TIME_COMPLEXITY @COPY_FROM _AddTargets(RConstIterator<LogTargetPtr> auto begin,
        ///     RConstIterator<LogTargetPtr> auto end)
        /// 
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        usize AddTargets(const RConstIterable<LogTargetPtr> auto& targets)
        {
            auto begin = targets.begin();
            auto end = targets.end();

            if (begin == end)
                return 0;

            LockGuard guard(_lock);
            _AddTargets(begin, end);
        }

        /// ----------------------------------------------------------------------------------------
        /// Adds LogTarget objects.
        /// 
        /// @PARAM[IN] targets LogTarget objects to add.
        ///     If {targets} contains null objects, this doesn't adds them.
        /// @RETURNS Count of LogTarget objects added.
        /// 
        /// @EXCEPTION_SAFETY @COPY_FROM _AddTargets(RConstIterator<LogTargetPtr> auto begin,
        ///     RConstIterator<LogTargetPtr> auto end)
        /// 
        /// @TIME_COMPLEXITY @COPY_FROM _AddTargets(RConstIterator<LogTargetPtr> auto begin,
        ///     RConstIterator<LogTargetPtr> auto end)
        /// 
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        usize AddTargets(InitializerList<LogTargetPtr> targets)
        {
            if (targets.size() == 0)
                return 0;

            LockGuard guard(_lock);
            _AddTargets(targets.begin(), targets.end());
        }

        /// ----------------------------------------------------------------------------------------
        /// Adds LogTarget objects.
        /// 
        /// @PARAM[IN] begin, end RConstIterator to beginning and end of range to add.
        ///     If range {[begin, end]} contains null objects, this doesn't adds them.
        /// @RETURNS Count of LogTarget objects added.
        /// 
        /// @EXCEPTION_SAFETY @COPY_FROM _AddTargets(RConstIterator<LogTargetPtr> auto begin,
        ///     RConstIterator<LogTargetPtr> auto end)
        /// 
        /// @TIME_COMPLEXITY @COPY_FROM _AddTargets(RConstIterator<LogTargetPtr> auto begin,
        ///     RConstIterator<LogTargetPtr> auto end)
        /// 
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        usize AddTargets(RConstIterator<LogTargetPtr> auto begin,
            RConstIterator<LogTargetPtr> auto end)
        {
            if (begin == end)
                return 0;

            LockGuard guard(_lock);
            _AddTargets(begin, end);
        }

        /// ----------------------------------------------------------------------------------------
        /// Removes LogTarget object if found.
        /// 
        /// @PARAM[IN] target LogTarget object to remove.
        ///     If {target} is null, this doesn't searches it.
        /// @RETURNS {true} if found and removed, else {false}.
        /// 
        /// @EXCEPTION_SAFETY @COPY_FROM _RemoveTarget(LogTargetPtr target)
        /// 
        /// @TIME_COMPLEXITY @COPY_FROM _RemoveTarget(LogTargetPtr target)
        /// 
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        bool RemoveTarget(LogTargetPtr target)
        {
            if (target == nullptr) return false;

            LockGuard guard(_lock);
            return _RemoveTarget(target);
        }

        /// ----------------------------------------------------------------------------------------
        /// Removes LogTarget objects if found.
        /// 
        /// @PARAM[IN] targets LogTarget objects to remove.
        ///     If {targets} contains null objects, this doesn't searches them.
        /// @RETURNS Count of LogTarget objects removed.
        /// 
        /// @EXCEPTION_SAFETY @COPY_FROM _RemoveTargets(RConstIterator<LogTargetPtr> auto begin,
        ///     RConstIterator<LogTargetPtr> auto end)
        /// 
        /// @TIME_COMPLEXITY @COPY_FROM _RemoveTargets(RConstIterator<LogTargetPtr> auto begin,
        ///     RConstIterator<LogTargetPtr> auto end)
        /// 
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        usize RemoveTargets(RIterable<LogTargetPtr> auto& targets)
        {
            auto begin = targets.begin();
            auto end = targets.end();

            if (begin == end)
                return 0;

            LockGuard guard(_lock);
            return _RemoveTargets(begin, end);
        }

        /// ----------------------------------------------------------------------------------------
        /// Removes LogTarget objects if found.
        /// 
        /// @PARAM[IN] targets LogTarget objects to remove.
        ///     If {targets} contains null objects, this doesn't searches them.
        /// @RETURNS Count of LogTarget objects removed.
        /// 
        /// @EXCEPTION_SAFETY @COPY_FROM _RemoveTargets(RConstIterator<LogTargetPtr> auto begin,
        ///     RConstIterator<LogTargetPtr> auto end)
        /// 
        /// @TIME_COMPLEXITY @COPY_FROM _RemoveTargets(RConstIterator<LogTargetPtr> auto begin,
        ///     RConstIterator<LogTargetPtr> auto end)
        /// 
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        usize RemoveTargets(InitializerList<LogTargetPtr> targets)
        {
            if (targets.size() == 0)
                return 0;

            LockGuard guard(_lock);
            return _RemoveTargets(targets.begin(), targets.end());
        }

        /// ----------------------------------------------------------------------------------------
        /// Removes LogTarget objects if found.
        /// 
        /// @PARAM[IN] begin RConstIterator to beginning of range to remove.
        /// @PARAM[IN] end RConstIterator to end of range to remove.
        ///     If range {[begin, end]} contains null objects, this doesn't searches them.
        /// @RETURNS Count of LogTarget objects removed.
        /// 
        /// @EXCEPTION_SAFETY @COPY_FROM _RemoveTargets(RConstIterator<LogTargetPtr> auto begin,
        ///     RConstIterator<LogTargetPtr> auto end)
        /// 
        /// @TIME_COMPLEXITY @COPY_FROM _RemoveTargets(RConstIterator<LogTargetPtr> auto begin,
        ///     RConstIterator<LogTargetPtr> auto end)
        /// 
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        usize RemoveTargets(RConstIterator<LogTargetPtr> auto begin,
            RConstIterator<LogTargetPtr> auto end)
        {
            LockGuard guard(_lock);
            return _RemoveTargets(begin, end);
        }

        /// ----------------------------------------------------------------------------------------
        /// Checks if this has LogTarget object.
        /// 
        /// @PARAM[IN] target LogTarget object to search for.
        ///     If {target} is null, this doesn't searches it.
        /// @RETURNS {true} if found, else {false}.
        /// 
        /// @TIME_COMPLEXITY @COPY_FROM _HasTarget(LogTargetPtr target)
        /// 
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        bool HasTarget(LogTargetPtr target) const noexcept
        {
            LockGuard guard(_lock);
            return _HasTarget(target);
        }

        /// ----------------------------------------------------------------------------------------
        /// Checks if this has LogTarget objects.
        /// 
        /// @PARAM[IN] target LogTarget objects to search for.
        ///     If {targets} contains null objects, this doesn't searches them.
        /// @RETURNS Count of LogTarget objects found.
        /// 
        /// @TIME_COMPLEXITY @COPY_FROM _HasTarget(LogTargetPtr target)
        /// 
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        usize HasTargets(RIterable<LogTargetPtr> auto& targets) const noexcept
        {
            LockGuard guard(_lock);
            return _HasTargets(targets.begin(), targets.end());
        }

        /// ----------------------------------------------------------------------------------------
        /// Checks if this has LogTarget objects.
        /// 
        /// @PARAM[IN] target LogTarget objects to search for.
        ///     If {targets} contains null objects, this doesn't searches them.
        /// @RETURNS Count of LogTarget objects found.
        /// 
        /// @TIME_COMPLEXITY @COPY_FROM _HasTarget(LogTargetPtr target)
        /// 
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        usize HasTargets(InitializerList<LogTargetPtr> targets) const noexcept
        {
            if (targets.size() == 0)
                return 0;

            LockGuard guard(_lock);
            return _HasTargets(targets.begin(), targets.end());
        }

        /// ----------------------------------------------------------------------------------------
        /// Checks if this has LogTarget objects.
        /// 
        /// @PARAM[IN] begin RConstIterator to beginning of range to search.
        /// @PARAM[IN] end RConstIterator to end of range to search.
        ///     If range {[begin, end]} contains null objects, this doesn't searches them.
        /// @RETURNS Count of LogTarget objects found.
        /// 
        /// @TIME_COMPLEXITY @COPY_FROM _HasTarget(LogTargetPtr target)
        /// 
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        usize HasTargets(RConstIterator<LogTargetPtr> auto begin,
            RConstIterator<LogTargetPtr> auto end)
            const noexcept
        {
            LockGuard guard(_lock);
            return _HasTargets(begin, end);
        }

        /// ----------------------------------------------------------------------------------------
        /// Reserves memory for {capacity} LogTarget objects.
        /// 
        /// @EXCEPTION_SAFETY @COPY_FROM _Reserve(usize capacity)
        /// 
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        void Reserve(usize capacity)
        {
            LockGuard guard(_lock);
            _Reserve(capacity);
        }

        /// ----------------------------------------------------------------------------------------
        /// Count of LogTarget objects this has.
        /// 
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        usize Count() const noexcept
        {
            LockGuard guard(_lock);
            return _targets.size();
        }

    //// -------------------------------------------------------------------------------------------
    //// Iteration
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// RConstIterator to beginning of iteration range.
        /// 
        /// @THREAD_SAFETY NONE
        /// 
        /// @TODO Make ThreadSafe.
        /// ----------------------------------------------------------------------------------------
        TConstIterator Begin() const noexcept
        {
            return _targets.cbegin();
        }

        /// ----------------------------------------------------------------------------------------
        /// RConstIterator to end of iteration range.
        /// 
        /// @THREAD_SAFETY NONE
        /// 
        /// @TODO Make ThreadSafe.
        /// ----------------------------------------------------------------------------------------
        TConstIterator End() const noexcept
        {
            return _targets.cend();
        }

    //// -------------------------------------------------------------------------------------------
    //// STD Iteration
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// @COPY_DOC Begin().
        /// ----------------------------------------------------------------------------------------
        TConstIterator begin() const noexcept
        {
            return Begin();
        }

        /// ----------------------------------------------------------------------------------------
        /// @COPY_DOC Begin().
        /// ----------------------------------------------------------------------------------------
        TConstIterator end() const noexcept
        {
            return End();
        }

    //// -------------------------------------------------------------------------------------------
    //// Implementation Functions
    //// 
    //// These functions doesn't perform any checks at runtime and assumes that all thread safety
    //// steps have been taken.
    //// -------------------------------------------------------------------------------------------

    protected:
        /// ----------------------------------------------------------------------------------------
        /// Adds the LogTarget object.
        /// 
        /// @PARAM[IN] target LogTarget object to add.
        ///     If {target} is null, this doesn't adds it.
        /// 
        /// @EXCEPTION_SAFETY @COPY_FROM ${Container}::InsertBack(LogTarget& target)
        /// 
        /// @TIME_COMPLEXITY @COPY_FROM ${Container}::InsertBack(LogTarget& target)
        /// 
        /// @THREAD_SAFETY NONE
        /// ----------------------------------------------------------------------------------------
        bool _AddTarget(LogTargetPtr target)
        {
            if (target == nullptr)
                return false;

            _targets.push_back(target);
            return true;
        }

        /// ----------------------------------------------------------------------------------------
        /// Adds the LogTarget objects.
        /// 
        /// @PARAM[IN] begin RConstIterator to beginning of range to add.
        /// @PARAM[IN] end RConstIterator to end of range to add.
        ///     If range {[begin, end]} contains null objects, this doesn't adds them.
        /// @RETURNS Count of LogTarget objects added.
        /// 
        /// @EXCEPTION_SAFETY @COPY_FROM ${Container}::InsertBack(LogTarget& target)
        ///  
        /// @TIME_COMPLEXITY @COPY_FROM ${Container}::InsertBack(LogTarget& target)
        /// 
        /// @THREAD_SAFETY NONE
        /// ----------------------------------------------------------------------------------------
        usize _AddTargets(RConstIterator<LogTargetPtr> auto begin,
            RConstIterator<LogTargetPtr> auto end)
        {
            usize count = 0;
            for (auto it = begin; it != end; it++)
            {
                if (*it == nullptr)
                    continue;

                _targets.push_back(*it);
                count++;
            }

            return count;
        }

        /// ----------------------------------------------------------------------------------------
        /// Removes the LogTarget object.
        /// 
        /// @PARAM[IN] target LogTarget object to remove.
        ///     If {target} is null, this doesn't searches it.
        /// @RETURNS {true} if found and removed, else {false}.
        /// 
        /// @EXCEPTION_SAFETY @COPY_FROM ${Container}::Remove(LogTarget& target)
        /// 
        /// @TIME_COMPLEXITY @COPY_FROM ${Container}::Remove(LogTarget& target)
        /// ----------------------------------------------------------------------------------------
        bool _RemoveTarget(LogTargetPtr target)
        {
            ATOM_DEBUG_ASSERT(target != nullptr);

            for (auto it = _targets.begin(); it != _targets.end(); it++)
            {
                if (*it == target)
                {
                    _targets.erase(it);
                    return true;
                }
            }

            return false;
        }

        /// ----------------------------------------------------------------------------------------
        /// Removes LogTarget objects.
        /// 
        /// @PARAM[IN] begin RConstIterator to beginning of the range to remove.
        /// @PARAM[IN] end RConstIterator to end of the range to remove.
        ///     If range {[begin, end]} contains null objects, this doesn't searches them.
        /// @RETURNS Count of LogTarget objects removed.
        /// 
        /// @EXCEPTION_SAFETY @COPY_FROM ${Container}::Remove(LogTarget& target)
        /// 
        /// @TIME_COMPLEXITY @COPY_FROM ${Container}::Remove(LogTarget& target)
        /// ----------------------------------------------------------------------------------------
        usize _RemoveTargets(RConstIterator<LogTargetPtr> auto begin,
            RConstIterator<LogTargetPtr> auto end)
        {
            usize removed = 0;
            for (auto otherIt = begin; otherIt != end; otherIt++)
            {
                for (auto it = _targets.begin(); it != _targets.end(); it++)
                {
                    if (*it == *otherIt)
                    {
                        it = _targets.erase(it);
                        removed++;
                        break;
                    }
                }
            }

            return removed;
        }

        /// ----------------------------------------------------------------------------------------
        /// Searches for LogTarget object.
        /// 
        /// @PARAM[IN] target LogTarget object to search for.
        ///     If {target} is null, doesn't searches it.
        /// @RETURNS {true} if found, else {false}.
        /// 
        /// @EXCEPTION_SAFETY NOEXCEPT
        /// 
        /// @TIME_COMPLEXITY Linear
        /// ----------------------------------------------------------------------------------------
        bool _HasTarget(LogTargetPtr target) const noexcept
        {
            if (target == nullptr) return false;

            for (auto it = _targets.begin(); it != _targets.end(); it++)
            {
                if (*it == target)
                {
                    return true;
                }
            }

            return false;
        }

        /// ----------------------------------------------------------------------------------------
        /// Search LogTarget objects.
        /// 
        /// @PARAM[IN] begin RConstIterator to beginning of range to search for.
        /// @PARAM[IN] end RConstIterator to end of range to search for.
        ///     If range {[begin, end]} contains null objects, this doesn't searches them.
        /// @RETURNS Count of LogTarget objects found.
        /// 
        /// @EXCEPTION_SAFETY NOEXCEPT
        /// 
        /// @TIME_COMPLEXITY Exponential
        /// ----------------------------------------------------------------------------------------
        usize _HasTargets(RConstIterator<LogTargetPtr> auto begin,
            RConstIterator<LogTargetPtr> auto end) const noexcept
        {
            usize found = 0;
            for (auto otherIt = begin; otherIt != end; otherIt++)
            {
                for (auto it = _targets.begin(); it != _targets.end(); it++)
                {
                    if (*it == *otherIt)
                    {
                        found++;
                        break;
                    }
                }
            }

            return found;
        }

        /// ----------------------------------------------------------------------------------------
        /// Reserves memory for {capacity} LogTarget objects.
        /// 
        /// @EXCEPTION_SAFETY @COPY_FROM ${Container}::Reserve(usize capacity).
        /// ----------------------------------------------------------------------------------------
        void _Reserve(usize capacity)
        {
            _targets.reserve(capacity);
        }

    protected:
        /// ----------------------------------------------------------------------------------------
        /// Container to store LogTarget objects.
        /// ----------------------------------------------------------------------------------------
        Container _targets;

        /// ----------------------------------------------------------------------------------------
        /// Lockable object for thread safety.
        /// ----------------------------------------------------------------------------------------
        TLockable _lock;
    };
}

namespace Atom::Logging
{
    using MultiLogTargetST = Internal::MultiLogTargetTemplate<NullLockable>;
    using MultiLogTargetMT = Internal::MultiLogTargetTemplate<SimpleMutex>;

    // STATIC_ASSERT(RConstIterable<MultiLogTargetST, LogTargetPtr>);
    // STATIC_ASSERT(RConstIterable<MultiLogTargetMT, LogTargetPtr>);
}