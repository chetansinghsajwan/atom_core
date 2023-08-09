#pragma once
#include "Atom/Memory.h"
#include "Atom/Containers.h"
#include "Atom/Exceptions.h"
#include "Atom/Logging/LogTarget.h"

namespace Atom::Logging::Private
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
    class MultiLogTargetTemplate : public LogTarget
    {
        using _TContainer = DynArr<LogTargetPtr>;

    public:
        using TElem = LogTargetPtr;
        using TIter = tname _TContainer::TIter;
        using TIterEnd = tname _TContainer::TIterEnd;

    public:
        /// ----------------------------------------------------------------------------------------
        /// Default constructor.
        /// ----------------------------------------------------------------------------------------
        ctor MultiLogTargetTemplate() noex { }

        /// ----------------------------------------------------------------------------------------
        /// Constructs with LogTarget objects.
        /// 
        /// @PARAM[IN] target RRange of LogTarget objects to add.
        ///     If {targets} contains null objects, this doesn't adds them.
        /// 
        /// @EXCEPTION_SAFETY @COPY_FROM _AddTargets(range).
        /// 
        /// @TIME_COMPLEXITY @COPY_FROM _AddTargets(range).
        /// ----------------------------------------------------------------------------------------
        template <tname TRange>
        requires RRangeOf<TRange, LogTargetPtr>
        ctor MultiLogTargetTemplate(const TRange& targets)
        {
            _AddTargets(mov(targets));
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
        virtual fn Write(const LogMsg& logMsg) -> void ofinal
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
        virtual fn Flush() -> void ofinal
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
        /// @PARAM[IN] target {LogTarget} object to add.
        ///     If {target} is null, this doesn't adds it.
        /// @RETURNS {true} if added, else {false}.
        /// 
        /// @EXCEPTION_SAFETY @COPY_FROM _AddTarget(LogTargetPtr target).
        /// 
        /// @TIME_COMPLEXITY @COPY_FROM _AddTarget(LogTargetPtr target).
        /// 
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        fn AddTarget(LogTargetPtr target) noex -> bool
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
        /// @EXCEPTION_SAFETY @COPY_FROM _AddTargets(range).
        /// 
        /// @TIME_COMPLEXITY @COPY_FROM _AddTargets(range).
        /// 
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        template <tname TRange>
        requires RRangeOf<TRange, LogTargetPtr>
        usize AddTargets(const TRange& targets)
        {
            if (!targets.iter() == targets.iterEnd())
                return 0;

            LockGuard guard(_lock);
            _AddTargets(targets);
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
        fn RemoveTarget(LogTargetPtr target) -> bool
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
        /// @EXCEPTION_SAFETY @COPY_FROM _RemoveTargets(const TRange& range)
        /// 
        /// @TIME_COMPLEXITY @COPY_FROM _RemoveTargets(const TRange& range)
        /// 
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        template <tname TRange>
        requires RRangeOf<TRange, LogTargetPtr>
        fn RemoveTargets(const TRange& targets) -> usize
        {
            if (!targets.iter() == targets.iterEnd())
                return 0;

            LockGuard guard(_lock);
            return _RemoveTargets(targets);
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
        fn HasTarget(LogTargetPtr target) const noex -> bool
        {
            if (target == nullptr)
                return false;

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
        template <tname TRange>
        requires RRangeOf<TRange, LogTargetPtr>
        fn HasTargets(const TRange& targets) const noex -> usize
        {
            if (!targets.iter() == targets.iterEnd())
                return 0;

            LockGuard guard(_lock);
            return _HasTargets(targets);
        }

        /// ----------------------------------------------------------------------------------------
        /// Reserves memory for {capacity} LogTarget objects.
        /// 
        /// @EXCEPTION_SAFETY @COPY_FROM _Reserve(usize capacity)
        /// 
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        fn Reserve(usize capacity)
        {
            LockGuard guard(_lock);
            _Reserve(capacity);
        }

        /// ----------------------------------------------------------------------------------------
        /// Count of LogTarget objects this has.
        /// 
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        fn Count() const noex -> usize
        {
            LockGuard guard(_lock);
            return _targets.Count();
        }

    //// -------------------------------------------------------------------------------------------
    //// Iteration
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// Iterator to the first LogTarget.
        /// 
        /// @THREAD_SAFETY NONE
        /// 
        /// @TODO Make ThreadSafe.
        /// ----------------------------------------------------------------------------------------
        fn iter() const noex -> TIter
        {
            return _targets.iter();
        }

        /// ----------------------------------------------------------------------------------------
        /// Iterator to the last LogTarget.
        /// 
        /// @THREAD_SAFETY NONE
        /// 
        /// @TODO Make ThreadSafe.
        /// ----------------------------------------------------------------------------------------
        fn iterEnd() const noex -> TIterEnd
        {
            return _targets.iterEnd();
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
        /// @PARAM[IN] target {LogTarget} object to add.
        ///     If {target} is null, this doesn't adds it.
        /// 
        /// @EXCEPTION_SAFETY @COPY_FROM ${_TContainer}::InsertBack(LogTarget& target)
        /// 
        /// @TIME_COMPLEXITY @COPY_FROM ${_TContainer}::InsertBack(LogTarget& target)
        /// 
        /// @THREAD_SAFETY NONE
        /// ----------------------------------------------------------------------------------------
        fn _AddTarget(LogTargetPtr target) -> bool
        {
            ATOM_DEBUG_EXPECTS(target != nullptr);

            return _targets.InsertBack(mov(target)) != _targets.iterEnd();
        }

        /// ----------------------------------------------------------------------------------------
        /// Adds the LogTarget objects.
        /// 
        /// @PARAM[IN] target {LogTarget} object to add.
        ///     If {targets} contains null objects, this doesn't adds them.
        /// @RETURNS Count of LogTarget objects added.
        /// 
        /// @EXCEPTION_SAFETY @COPY_FROM ${_TContainer}::InsertBack(RRangeOf<LogTargetPtr> targets)
        ///  
        /// @TIME_COMPLEXITY @COPY_FROM ${_TContainer}::InsertBack(RRangeOf<LogTargetPtr> targets)
        /// 
        /// @THREAD_SAFETY NONE
        /// ----------------------------------------------------------------------------------------
        template <tname TRange>
        requires RRangeOf<TRange, LogTargetPtr>
        fn _AddTargets(const TRange& targets) -> usize
        {
            usize count = 0;
            for (LogTargetPtr target : targets)
            {
                if (target != nullptr)
                {
                    _targets.InsertBack(mov(target));
                    count++;
                }
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
        /// @EXCEPTION_SAFETY @COPY_FROM ${_TContainer}::Remove(LogTarget& target)
        /// 
        /// @TIME_COMPLEXITY @COPY_FROM ${_TContainer}::Remove(LogTarget& target)
        /// ----------------------------------------------------------------------------------------
        fn _RemoveTarget(LogTargetPtr target) -> bool
        {
            ATOM_DEBUG_EXPECTS(target != nullptr);

            for (auto it = _targets.iter(); it != _targets.iterEnd(); it++)
            {
                if (*it == target)
                {
                    _targets.RemoveAt(it);
                    return true;
                }
            }

            return false;
        }

        /// ----------------------------------------------------------------------------------------
        /// Removes LogTarget objects.
        /// 
        /// @PARAM[IN] it RRange to beginning of the range to remove.
        /// @PARAM[IN] end RRange to end of the range to remove.
        ///     If range {[it, end]} contains null objects, this doesn't searches them.
        /// @RETURNS Count of LogTarget objects removed.
        /// 
        /// @EXCEPTION_SAFETY @COPY_FROM ${_TContainer}::Remove(LogTarget& target)
        /// 
        /// @TIME_COMPLEXITY @COPY_FROM ${_TContainer}::Remove(LogTarget& target)
        /// ----------------------------------------------------------------------------------------
        template <tname TRange>
        requires RRangeOf<TRange, LogTargetPtr>
        fn _RemoveTargets(const TRange& targets) -> usize
        {
            usize count = 0;
            for (auto target : targets)
            {
                if (target == nullptr)
                    continue;

                for (auto it = _targets.iter(); it != _targets.iterEnd(); it++)
                {
                    if (*it == target)
                    {
                        _targets.RemoveAt(it);
                        count++;
                        break;
                    }
                }
            }

            return count;
        }

        /// ----------------------------------------------------------------------------------------
        /// Searches for LogTarget object.
        /// 
        /// @PARAM[IN] target LogTarget object to search for.
        ///     If {target} is null, doesn't searches it.
        /// @RETURNS {true} if found, else {false}.
        /// 
        /// @EXCEPTION_SAFETY noex
        /// 
        /// @TIME_COMPLEXITY Linear
        /// ----------------------------------------------------------------------------------------
        fn _HasTarget(const LogTargetPtr& target) const noex -> bool
        {
            ATOM_DEBUG_EXPECTS(target != nullptr);

            return RangeFinder().Contains(_targets, target);
        }

        /// ----------------------------------------------------------------------------------------
        /// Search LogTarget objects.
        /// 
        /// @PARAM[IN] it RRange to beginning of range to search for.
        /// @PARAM[IN] end RRange to end of range to search for.
        ///     If range {[it, end]} contains null objects, this doesn't searches them.
        /// @RETURNS Count of LogTarget objects found.
        /// 
        /// @EXCEPTION_SAFETY noex
        /// 
        /// @TIME_COMPLEXITY Exponential
        /// ----------------------------------------------------------------------------------------
        template <tname TRange>
        requires RRangeOf<TRange, LogTargetPtr>
        fn _HasTargets(const TRange& targets) -> usize
        {
            return RangeFinder().Contains(_targets, targets);
        }

        /// ----------------------------------------------------------------------------------------
        /// Reserves memory for {capacity} LogTarget objects.
        /// 
        /// @EXCEPTION_SAFETY @COPY_FROM ${_TContainer}::Reserve(usize capacity).
        /// ----------------------------------------------------------------------------------------
        fn _Reserve(usize capacity)
        {
            _targets.Reserve(capacity);
        }

    protected:
        /// ----------------------------------------------------------------------------------------
        /// _TContainer to store LogTarget objects.
        /// ----------------------------------------------------------------------------------------
        _TContainer _targets;

        /// ----------------------------------------------------------------------------------------
        /// Lockable object for thread safety.
        /// ----------------------------------------------------------------------------------------
        TLockable _lock;
    };
}

namespace Atom::Logging
{
    using MultiLogTargetST = Private::MultiLogTargetTemplate<NullLockable>;
    using MultiLogTargetMT = Private::MultiLogTargetTemplate<SimpleMutex>;

    static_assert(RFwdRangeOf<MultiLogTargetST, LogTargetPtr>,
        "{MultiLogTargetST} does not meet {RFwdRange} requirements.");

    static_assert(RFwdRangeOf<MultiLogTargetMT, LogTargetPtr>,
        "{MultiLogTargetMT} does not meet {RFwdRange} requirements.");
}