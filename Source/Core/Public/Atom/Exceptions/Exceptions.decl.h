#pragma once
#include <source_location>
// #include <stacktrace>

#include "Atom/Str/Str.decl.h"
#include "Atom/Str/StrView.decl.h"
#include "Atom/Str/AsciiStrView.decl.h"

namespace Atom
{
    /// --------------------------------------------------------------------------------------------
    /// Details about the origin of exception.
    /// --------------------------------------------------------------------------------------------
    struct ExceptionSource
    {
        using TImpl = std::source_location;

        static constexpr ExceptionSource Current(TImpl src = TImpl::current()) noex;

        uint32 line;
        uint32 column;
        AsciiStrView fileName;
        AsciiStrView funcName;
    };

    /// --------------------------------------------------------------------------------------------
    /// A record of stack frames.
    /// 
    /// @FIX: GCC doesn't has stacktrace.
    /// --------------------------------------------------------------------------------------------
    // using StackTrace = std::stacktrace;
    struct StackTrace
    {
        static constexpr StackTrace current() noex
        {
            return StackTrace{ };
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// Base class for all exceptions.
    /// --------------------------------------------------------------------------------------------
    class Exception: public std::exception
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// Basic constructor with msg.
        /// ----------------------------------------------------------------------------------------
        Exception(Str msg) noex;

        /// ----------------------------------------------------------------------------------------
        /// VirtualDestructor.
        /// ----------------------------------------------------------------------------------------
        virtual ~Exception() noex;

    public:
        /// ----------------------------------------------------------------------------------------
        /// Name of the exception.
        /// ----------------------------------------------------------------------------------------
        StrView exName;

        /// ----------------------------------------------------------------------------------------
        /// Message explaining what went wrong.
        /// ----------------------------------------------------------------------------------------
        Str msg;

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
        using Exception::Exception;
    };

    /// --------------------------------------------------------------------------------------------
    /// Exception represents an out of range access, where range is representable using indices.
    /// --------------------------------------------------------------------------------------------
    struct IndexOutOfRangeException: OutOfRangeException
    {
        IndexOutOfRangeException(Str msg, usize index, usize begin, usize end);

        usize index;
        usize begin;
        usize end;
    };
}

namespace Atom::Ex::Internal
{
    class Thrower
    {
    public:
        Thrower(ExceptionSource src = ExceptionSource::Current(),
            StackTrace stackTrace = StackTrace::current()) noex;

        Thrower& RecordStack(StackTrace stackTrace = StackTrace::current());

        Thrower& RecordSource(ExceptionSource src = ExceptionSource::Current());

        template <tname TEx>
        requires RDerivedFrom<TEx, Exception>
        ATOM_ATTR_NORETURN
        void operator << (TEx&& ex);

    protected:
        ExceptionSource src;
        StackTrace stackTrace;
    };
}

/// ------------------------------------------------------------------------------------------------
/// ATOM_THROW
/// ------------------------------------------------------------------------------------------------
#define ATOM_THROW ::Atom::Ex::Internal::Thrower()
