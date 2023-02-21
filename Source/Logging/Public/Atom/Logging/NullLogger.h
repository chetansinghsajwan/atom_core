#pragma once
#include "Atom/Logging/Logger.h"

namespace Atom::Logging
{
    /// --------------------------------------------------------------------------------------------
    /// NullLogger is used in cases where it's not appropriate to return nullptr.
    /// 
    /// NullLogger is stateless and doesn't do anything except refusing every log request.
    /// Damn rude nigga!.
    /// --------------------------------------------------------------------------------------------
    class NullLogger final: public Logger
    {
    //// -------------------------------------------------------------------------------------------
    //// Static Members
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// Static instance of this type because this type is stateless.
        /// It's recommended to use this instead of creating one each time.
        /// ----------------------------------------------------------------------------------------
        static LoggerPtr Instance;

    private:
        /// ----------------------------------------------------------------------------------------
        /// Name of the NullLogger.
        /// ----------------------------------------------------------------------------------------
        static constexpr StringView s_name = "NullLogger";

    //// -------------------------------------------------------------------------------------------
    //// NonStatic Members
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// Default constructs NullLogger.
        /// ----------------------------------------------------------------------------------------
        constexpr NullLogger() noexcept { }

    public:
        /// ----------------------------------------------------------------------------------------
        /// Returns name of the NullLogger.
        /// ----------------------------------------------------------------------------------------
        constexpr virtual StringView Name() const noexcept override
        {
            return s_name;
        }

        /// ----------------------------------------------------------------------------------------
        /// Does nothing.
        /// ----------------------------------------------------------------------------------------
        constexpr virtual void Log(LogMsg& logMsg) override { }

        /// ----------------------------------------------------------------------------------------
        /// Does nothing.
        /// ----------------------------------------------------------------------------------------
        constexpr virtual void Flush() override { }

        /// ----------------------------------------------------------------------------------------
        /// Always returns false to avoid constructing log msg.
        /// ----------------------------------------------------------------------------------------
        constexpr virtual bool ShouldLog(LogLevel lvl) const noexcept override
        {
            return false;
        }
    };

    LoggerPtr NullLogger::Instance = MakeShared<NullLogger>();
}