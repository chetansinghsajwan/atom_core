#pragma once
#include <source_location>
#include <stacktrace>

#include "Atom/String/String.decl.h"
#include "Atom/String/StringView.decl.h"
#include "Atom/String/ASCIIStringView.decl.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// Details about the origin of exception.
    /// --------------------------------------------------------------------------------------------
    struct ExceptionSource
    {
        using TImpl = std::source_location;

        static consteval ExceptionSource Current(TImpl src = TImpl::current()) noexcept
        {
            return ExceptionSource
            {
                .line = src.line(),
                .column = src.column(),
                .fileName = src.file_name(),
                .funcName = src.function_name()
            };
        }

        uint32 line;
        uint32 column;
        ASCIIStringView fileName;
        ASCIIStringView funcName;
    };

    /// --------------------------------------------------------------------------------------------
    /// A record of stack frames.
    /// 
    /// TODO: Implement this.
    /// --------------------------------------------------------------------------------------------
    using StackTrace = std::stacktrace;

    /// --------------------------------------------------------------------------------------------
    /// Base class for all exceptions.
    /// --------------------------------------------------------------------------------------------
    class Exception: public std::exception
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// Basic constructor with msg.
        /// ----------------------------------------------------------------------------------------
        Exception(String msg) noexcept;

        /// ----------------------------------------------------------------------------------------
        /// VirtualDestructor.
        /// ----------------------------------------------------------------------------------------
        virtual ~Exception() noexcept;

    public:
        /// ----------------------------------------------------------------------------------------
        /// Name of the exception.
        /// ----------------------------------------------------------------------------------------
        StringView exName;

        /// ----------------------------------------------------------------------------------------
        /// Message explaining what went wrong.
        /// ----------------------------------------------------------------------------------------
        String msg;

        /// ----------------------------------------------------------------------------------------
        /// Origin of the exception.
        /// ----------------------------------------------------------------------------------------
        ExceptionSource src;

        /// ----------------------------------------------------------------------------------------
        /// StackTrace when exception was thrown.
        /// ----------------------------------------------------------------------------------------
        StackTrace stackTrace;
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    struct RuntimeException: Exception
    {
        using Exception::Exception;
    };

    /// --------------------------------------------------------------------------------------------
    /// Exception represents invalid operation.
    /// --------------------------------------------------------------------------------------------
    struct InvalidOperationException: Exception
    {
        using Exception::Exception;
    };

    /// --------------------------------------------------------------------------------------------
    /// Exception represents invalid argument.
    /// --------------------------------------------------------------------------------------------
    struct InvalidArgumentException: Exception
    {
        using Exception::Exception;
    };

    /// --------------------------------------------------------------------------------------------
    /// Exception represents null pointer access.
    /// --------------------------------------------------------------------------------------------
    struct NullPointerException: Exception
    {
        using Exception::Exception;
    };

    /// --------------------------------------------------------------------------------------------
    /// Exception represents an out of range access.
    /// --------------------------------------------------------------------------------------------
    struct OutOfRangeException: Exception
    {
        using Exception::Exception;;

        usize pos;
        usize begin;
        usize end;
    };
}

namespace Atom::Ex::Internal
{
    class Thrower
    {
    public:
        constexpr Thrower(ExceptionSource src = ExceptionSource::Current()) noexcept :
            src{ src }, stackTrace{ } { }

        Thrower& RecordStack(StackTrace stackTrace = StackTrace::current())
        {
            this->stackTrace = MOVE(stackTrace);
            return *this;
        }

        Thrower& RecordSource(ExceptionSource src = ExceptionSource::Current())
        {
            this->src = MOVE(src);
            return *this;
        }

        template <typename TEx>
        requires RDerivedFrom<TEx, Exception>
        void operator << (TEx&& ex)
        {
            ex.src = MOVE(src);
            ex.stackTrace = MOVE(stackTrace);

            throw ex;
        }

        ExceptionSource src;
        StackTrace stackTrace;
    };
}

/// ------------------------------------------------------------------------------------------------
/// ATOM_THROW
/// ------------------------------------------------------------------------------------------------
#define ATOM_THROW ::Atom::Ex::Internal::Thrower()