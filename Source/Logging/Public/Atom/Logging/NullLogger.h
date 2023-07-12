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
        static constexpr StrView s_name = TEXT("NullLogger");

    //// -------------------------------------------------------------------------------------------
    //// NonStatic Members
    //// -------------------------------------------------------------------------------------------

    public:
        /// ----------------------------------------------------------------------------------------
        /// Default constructs NullLogger.
        /// ----------------------------------------------------------------------------------------
        constexpr ctor NullLogger() noex { }

    public:
        /// ----------------------------------------------------------------------------------------
        /// Returns name of the NullLogger.
        /// ----------------------------------------------------------------------------------------
        constexpr virtual fn Name() const noex -> StrView override
        {
            return s_name;
        }

        /// ----------------------------------------------------------------------------------------
        /// Does nothing.
        /// ----------------------------------------------------------------------------------------
        constexpr virtual fn Log(LogMsg& logMsg) -> void override { }

        /// ----------------------------------------------------------------------------------------
        /// Does nothing.
        /// ----------------------------------------------------------------------------------------
        constexpr virtual fn Flush() -> void override { }

        /// ----------------------------------------------------------------------------------------
        /// Always returns false to avoid constructing log msg.
        /// ----------------------------------------------------------------------------------------
        constexpr virtual fn CheckLogLevel(ELogLevel lvl) const noex -> bool override
        {
            return false;
        }
    };

    inline LoggerPtr NullLogger::Instance = MakeShared<NullLogger>();
}