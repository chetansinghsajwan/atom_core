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
        using TIter = typename _TContainer::TIter;
        using TIterEnd = typename _TContainer::TIterEnd;

    public:
        /// ----------------------------------------------------------------------------------------
        /// Default constructor.
        /// ----------------------------------------------------------------------------------------
        MultiLogTargetTemplate() noexcept { }

        /// ----------------------------------------------------------------------------------------
        /// Constructs and adds LogTarget object.
        /// 
        /// @PARAM[IN] target {LogTarget} object to add.
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
        /// @PARAM[IN] targets InitList of LogTarget objects to add.
        ///     If {targets} contains null objects, this doesn't adds them.
        /// 
        /// @EXCEPTION_SAFETY @COPY_FROM _AddTargets(targets)
        /// 
        /// @TIME_COMPLEXITY @COPY_FROM _AddTargets(targets)
        /// ----------------------------------------------------------------------------------------
        MultiLogTargetTemplate(InitList<LogTargetPtr> targets)
        {
            _AddTargets(ArrIter<LogTargetPtr>(targets));
        }

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
        template <typename TRange>
        requires RRange<TRange, LogTargetPtr>
        MultiLogTargetTemplate(const TRange& targets)
        {
            _AddTargets(MOVE(targets));
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
        /// @EXCEPTION_SAFETY @COPY_FROM _AddTargets(range).
        /// 
        /// @TIME_COMPLEXITY @COPY_FROM _AddTargets(range).
        /// 
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        requires RRange<TRange, LogTargetPtr>
        usize AddTargets(const TRange& targets)
        {
            if (!targets.Iter() == targets.IterEnd())
                return 0;

            LockGuard guard(_lock);
            _AddTargets(targets);
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
        usize AddTargets(InitList<LogTargetPtr> targets)
        {
            return AddTargets(ArrIter<LogTargetPtr>(targets));
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
        /// @EXCEPTION_SAFETY @COPY_FROM _RemoveTargets(const TRange& range)
        /// 
        /// @TIME_COMPLEXITY @COPY_FROM _RemoveTargets(const TRange& range)
        /// 
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        requires RRange<TRange, LogTargetPtr>
        usize RemoveTargets(const TRange& targets)
        {
            if (!targets.Iter() == targets.IterEnd())
                return 0;

            LockGuard guard(_lock);
            return _RemoveTargets(targets);
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
        usize RemoveTargets(InitList<LogTargetPtr> targets)
        {
            return RemoveTargets(ArrIter<LogTargetPtr>(targets));
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
        template <typename TRange>
        requires RRange<TRange, LogTargetPtr>
        usize HasTargets(const TRange& targets) const noexcept
        {
            if (!targets.Iter() == targets.IterEnd())
                return 0;

            LockGuard guard(_lock);
            return _HasTargets(targets);
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
        usize HasTargets(InitList<LogTargetPtr> targets) const noexcept
        {
            return HasTargets(ArrIter<LogTargetPtr>(targets));
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
        TIter Iter() const noexcept
        {
            return _targets.Iter();
        }

        /// ----------------------------------------------------------------------------------------
        /// Iterator to the last LogTarget.
        /// 
        /// @THREAD_SAFETY NONE
        /// 
        /// @TODO Make ThreadSafe.
        /// ----------------------------------------------------------------------------------------
        TIter IterEnd() const noexcept
        {
            return _targets.IterEnd();
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
        bool _AddTarget(LogTargetPtr target)
        {
            ATOM_DEBUG_EXPECTS(target != nullptr);

            return _targets.InsertBack(MOVE(target)) != _targets.IterEnd();
        }

        /// ----------------------------------------------------------------------------------------
        /// Adds the LogTarget objects.
        /// 
        /// @PARAM[IN] target {LogTarget} object to add.
        ///     If {targets} contains null objects, this doesn't adds them.
        /// @RETURNS Count of LogTarget objects added.
        /// 
        /// @EXCEPTION_SAFETY @COPY_FROM ${_TContainer}::InsertBack(RRange<LogTargetPtr> targets)
        ///  
        /// @TIME_COMPLEXITY @COPY_FROM ${_TContainer}::InsertBack(RRange<LogTargetPtr> targets)
        /// 
        /// @THREAD_SAFETY NONE
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        requires RRange<TRange, LogTargetPtr>
        usize _AddTargets(const TRange& targets)
        {
            return _targets.InsertBack(targets,
                [](const LogTargetPtr& target)
                {
                    return target != nullptr;
                }).Range();
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
        bool _RemoveTarget(LogTargetPtr in_target)
        {
            ATOM_DEBUG_EXPECTS(in_target != nullptr);

            return _targets.Remove(in_target);
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
        template <typename TRange>
        requires RRange<TRange, LogTargetPtr>
        usize _RemoveTargets(const TRange& targets)
        {
            return _targets.Remove(targets, [](const LogTargetPtr& target)
                {
                    return target != nullptr;
                });
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
        bool _HasTarget(const LogTargetPtr& target) const noexcept
        {
            ATOM_DEBUG_EXPECTS(target != nullptr);

            return _targets.Contains(target);
        }

        /// ----------------------------------------------------------------------------------------
        /// Search LogTarget objects.
        /// 
        /// @PARAM[IN] it RRange to beginning of range to search for.
        /// @PARAM[IN] end RRange to end of range to search for.
        ///     If range {[it, end]} contains null objects, this doesn't searches them.
        /// @RETURNS Count of LogTarget objects found.
        /// 
        /// @EXCEPTION_SAFETY NOEXCEPT
        /// 
        /// @TIME_COMPLEXITY Exponential
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        requires RRange<TRange, LogTargetPtr>
        usize _HasTargets(const TRange& targets)
        {
            return _targets.Contains(targets);
        }

        /// ----------------------------------------------------------------------------------------
        /// Reserves memory for {capacity} LogTarget objects.
        /// 
        /// @EXCEPTION_SAFETY @COPY_FROM ${_TContainer}::Reserve(usize capacity).
        /// ----------------------------------------------------------------------------------------
        void _Reserve(usize capacity)
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

    static_assert(RFwdRange<MultiLogTargetST, LogTargetPtr>,
        "{MultiLogTargetST} does not meet {RFwdRange} requirements.");

    static_assert(RFwdRange<MultiLogTargetMT, LogTargetPtr>,
        "{MultiLogTargetMT} does not meet {RFwdRange} requirements.");
}