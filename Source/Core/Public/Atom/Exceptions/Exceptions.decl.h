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
        pub using TImpl = std::source_location;

        pub static cexpr ExceptionSource Current(TImpl src = TImpl::current());

        pub u32 line;
        pub u32 column;
        pub AsciiStrView fileName;
        pub AsciiStrView funcName;
    };

    /// --------------------------------------------------------------------------------------------
    /// A record of stack frames.
    /// 
    /// @FIX: GCC doesn't has stacktrace.
    /// --------------------------------------------------------------------------------------------
    // using StackTrace = std::stacktrace;
    class StackTrace
    {
        pub static cexpr StackTrace current()
        {
            return StackTrace{ };
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// Base class for all exceptions.
    /// --------------------------------------------------------------------------------------------
    class Exception extends std::exception
    {
        /// ----------------------------------------------------------------------------------------
        /// Basic constructor with msg.
        /// ----------------------------------------------------------------------------------------
        pub ctor Exception() = default;

        /// ----------------------------------------------------------------------------------------
        /// Basic constructor with msg.
        /// ----------------------------------------------------------------------------------------
        pub ctor Exception(Str msg);

        /// ----------------------------------------------------------------------------------------
        /// VirtualDestructor.
        /// ----------------------------------------------------------------------------------------
        pub virtual dtor Exception();

        /// ----------------------------------------------------------------------------------------
        /// Name of the exception.
        /// ----------------------------------------------------------------------------------------
        pub StrView exName;

        /// ----------------------------------------------------------------------------------------
        /// Message explaining what went wrong.
        /// ----------------------------------------------------------------------------------------
        pub Str msg;

        /// ----------------------------------------------------------------------------------------
        /// Origin of the exception.
        /// ----------------------------------------------------------------------------------------
        pub ExceptionSource src;

        /// ----------------------------------------------------------------------------------------
        /// StackTrace when exception was thrown.
        /// ----------------------------------------------------------------------------------------
        pub StackTrace stackTrace;
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    class RuntimeException extends Exception
    {
        pub using Exception::Exception;
    };

    /// --------------------------------------------------------------------------------------------
    /// Exception represents invalid operation.
    /// --------------------------------------------------------------------------------------------
    class InvalidOperationException extends Exception
    {
        pub using Exception::Exception;
    };

    /// --------------------------------------------------------------------------------------------
    /// Exception represents invalid argument.
    /// --------------------------------------------------------------------------------------------
    class InvalidArgumentException extends Exception
    {
        pub using Exception::Exception;
    };

    /// --------------------------------------------------------------------------------------------
    /// Exception represents null pointer access.
    /// --------------------------------------------------------------------------------------------
    class NullPointerException extends Exception
    {
        pub using Exception::Exception;
    };

    /// --------------------------------------------------------------------------------------------
    /// Exception represents an out of range access.
    /// --------------------------------------------------------------------------------------------
    class OutOfRangeException extends Exception
    {
        pub using Exception::Exception;
    };

    /// --------------------------------------------------------------------------------------------
    /// Exception represents an out of range access, where range is representable using indices.
    /// --------------------------------------------------------------------------------------------
    class IndexOutOfRangeException extends OutOfRangeException
    {
        pub ctor IndexOutOfRangeException(Str msg, usize index, usize begin, usize end);

        pub ctor IndexOutOfRangeException(usize index, usize begin, usize end);

        pub usize index;
        pub usize begin;
        pub usize end;
    };
}

namespace Atom::Ex::Internal
{
    class Thrower
    {
        pub ctor Thrower(ExceptionSource src = ExceptionSource::Current(),
            StackTrace stackTrace = StackTrace::current());

        pub fn RecordStack(StackTrace stackTrace = StackTrace::current()) -> Thrower&;

        pub fn RecordSource(ExceptionSource src = ExceptionSource::Current()) -> Thrower&;

        pub template <typename TEx>
        requires RDerivedFrom<TEx, Exception>
        noret fn op<<(TEx&& ex);

        prot ExceptionSource _src;
        prot StackTrace _stackTrace;
    };
}

/// ------------------------------------------------------------------------------------------------
/// ATOM_THROW
/// ------------------------------------------------------------------------------------------------
#define ATOM_THROW ::Atom::Ex::Internal::Thrower()
