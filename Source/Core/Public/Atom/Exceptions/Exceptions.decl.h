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
    class ExceptionSource
    {
    public:
        using TImpl = std::source_location;

    public:
        static constexpr ExceptionSource Current(TImpl src = TImpl::current());

    public:
        u32 line;
        u32 column;
        AsciiStrView fileName;
        AsciiStrView funcName;
    };

    /// --------------------------------------------------------------------------------------------
    /// A record of stack frames.
    /// 
    /// @FIX: GCC doesn't has stacktrace.
    /// --------------------------------------------------------------------------------------------
    // using StackTrace = std::stacktrace;
    class StackTrace
    {
    public:
        static constexpr StackTrace current()
        {
            return StackTrace{ };
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// Base class for all exceptions.
    /// --------------------------------------------------------------------------------------------
    class Exception : public std::exception
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// Basic constructor with msg.
        /// ----------------------------------------------------------------------------------------
        Exception() = default;

        /// ----------------------------------------------------------------------------------------
        /// Basic constructor with msg.
        /// ----------------------------------------------------------------------------------------
        Exception(Str msg);

        /// ----------------------------------------------------------------------------------------
        /// VirtualDestructor.
        /// ----------------------------------------------------------------------------------------
        virtual ~Exception();

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
    class RuntimeException : public Exception
    {
    public:
        using Exception::Exception;
    };

    /// --------------------------------------------------------------------------------------------
    /// Exception represents invalid operation.
    /// --------------------------------------------------------------------------------------------
    class InvalidOperationException : public Exception
    {
    public:
        using Exception::Exception;
    };

    /// --------------------------------------------------------------------------------------------
    /// Exception represents invalid argument.
    /// --------------------------------------------------------------------------------------------
    class InvalidArgumentException : public Exception
    {
    public:
        using Exception::Exception;
    };

    /// --------------------------------------------------------------------------------------------
    /// Exception represents null pointer access.
    /// --------------------------------------------------------------------------------------------
    class NullPointerException : public Exception
    {
    public:
        using Exception::Exception;
    };

    /// --------------------------------------------------------------------------------------------
    /// Exception represents an out of range access.
    /// --------------------------------------------------------------------------------------------
    class OutOfRangeException : public Exception
    {
    public:
        using Exception::Exception;
    };

    /// --------------------------------------------------------------------------------------------
    /// Exception represents an out of range access, where range is representable using indices.
    /// --------------------------------------------------------------------------------------------
    class IndexOutOfRangeException : public OutOfRangeException
    {
    public:
        IndexOutOfRangeException(Str msg, usize index, usize begin, usize end);

        IndexOutOfRangeException(usize index, usize begin, usize end);

    public:
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
            StackTrace stackTrace = StackTrace::current());

    public:
        auto RecordStack(StackTrace stackTrace = StackTrace::current()) -> Thrower&;

        auto RecordSource(ExceptionSource src = ExceptionSource::Current()) -> Thrower&;

        template <typename TEx>
        [[noreturn]]
        auto operator<<(TEx&& ex)
            requires(RDerivedFrom<TEx, Exception>);

    protected:
        ExceptionSource _src;
        StackTrace _stackTrace;
    };
}

/// ------------------------------------------------------------------------------------------------
/// ATOM_THROW
/// ------------------------------------------------------------------------------------------------
#define ATOM_THROW ::Atom::Ex::Internal::Thrower()
