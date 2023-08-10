#pragma once
#include "Atom/Logging/Logger.h"
#include "Atom/Logging/LogTarget.h"
#include "Atom/Logging/LogTargets/MultiLogTarget.h"

namespace Atom::Logging::Internal
{
    /// --------------------------------------------------------------------------------------------
    /// --- DOC_TEMPLATE 
    /// --------------------------------------------------------------------------------------------
    /// Basic Logger object.
    /// 
    /// @THREAD_SAFETY SAFE
    /// --------------------------------------------------------------------------------------------
    template <bool ST>
    class SimpleLoggerTemplate: public Logger
    {
        using MultiLogTarget = TTI::TConditional<ST, MultiLogTargetST, MultiLogTargetMT>;
        using AtomicLogLevel = TTI::TConditional<ST, ELogLevel, Atomic<ELogLevel>>;

    public:
        /// ----------------------------------------------------------------------------------------
        /// DefaultConstructor.
        /// 
        /// @PARAM[IN] name Name of this logger.
        /// ----------------------------------------------------------------------------------------
        explicit ctor SimpleLoggerTemplate(Str name):
            _name(mov(name)), targets() { }

        /// ----------------------------------------------------------------------------------------
        /// Constructs and adds LogTarget objects.
        /// Constructs {targets(targets)}.
        /// 
        /// @PARAM[IN] name Name of this logger.
        /// @PARAM[IN] targets LogTarget objects to add.
        /// ----------------------------------------------------------------------------------------
        template <typename TRange>
        ctor SimpleLoggerTemplate(Str name, const TRange& targets)
            requires(RRangeOf<TRange, LogTargetPtr>):
            _name(mov(name)), targets(targets) { }

    public:
        /// ----------------------------------------------------------------------------------------
        /// Get the name of this Logger.
        /// 
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        fn Name() const -> StrView ofinal
        {
            return _name;
        }

        /// ----------------------------------------------------------------------------------------
        /// Writes to all LogTarget objects.
        /// 
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        fn Log(LogMsg& logMsg) -> void override
        {
            targets.Write(logMsg);
        }

        /// ----------------------------------------------------------------------------------------
        /// Flushes all LogTarget objects.
        /// 
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        fn Flush() -> void override
        {
            targets.Flush();
        }

        /// ----------------------------------------------------------------------------------------
        /// Set log level.
        /// 
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        fn SetLogLevel(ELogLevel lvl)
        {
            _logLevel = lvl;
        }

        /// ----------------------------------------------------------------------------------------
        /// Get log level.
        /// 
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        fn GetLogLevel(ELogLevel lvl) -> ELogLevel
        {
            return _logLevel;
        }

        /// ----------------------------------------------------------------------------------------
        /// Checks if Log of level {lvl} should be logged.
        /// 
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        fn CheckLogLevel(ELogLevel lvl) const -> bool ofinal
        {
            return lvl != ELogLevel::OFF && lvl >= _logLevel;
        }

        /// ----------------------------------------------------------------------------------------
        /// Set flush level.
        /// 
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        fn SetFlushLevel(ELogLevel lvl)
        {
            _flushLevel = lvl;
        }

        /// ----------------------------------------------------------------------------------------
        /// Get flush level.
        /// 
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        fn GetFlushLevel() const -> ELogLevel
        {
            return _flushLevel;
        }

        /// ----------------------------------------------------------------------------------------
        /// Checks if Log of level {lvl} should be flushed.
        /// 
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        fn CheckFlushLevel(ELogLevel lvl) const -> bool
        {
            return lvl != ELogLevel::OFF && lvl >= _flushLevel;
        }

    public:
        /// ----------------------------------------------------------------------------------------
        /// The object used to manage LogTarget objects. Accessing it is thread safe.
        /// 
        /// @THREAD_SAFETY SAFE
        /// ----------------------------------------------------------------------------------------
        MultiLogTarget targets;

    protected:
        /// ----------------------------------------------------------------------------------------
        /// Name of this logger.
        /// ----------------------------------------------------------------------------------------
        const Str _name;

        /// ----------------------------------------------------------------------------------------
        /// ELogLevel used to filter logs.
        /// ----------------------------------------------------------------------------------------
        AtomicLogLevel _logLevel;

        /// ----------------------------------------------------------------------------------------
        /// ELogLevel used to filter flush after logs.
        /// ----------------------------------------------------------------------------------------
        AtomicLogLevel _flushLevel;
    };
}

namespace Atom::Logging
{
    using SimpleLoggerST = Internal::SimpleLoggerTemplate<true>;
    using SimpleLoggerMT = Internal::SimpleLoggerTemplate<false>;
}