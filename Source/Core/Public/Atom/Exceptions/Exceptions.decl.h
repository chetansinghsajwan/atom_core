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

        static constexpr ExceptionSource Current(TImpl src = TImpl::current()) noexcept;

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
        using Exception::Exception;
    };

    /// --------------------------------------------------------------------------------------------
    /// Exception represents an out of range access, where range is representable using indices.
    /// --------------------------------------------------------------------------------------------
    struct IndexOutOfRangeException: OutOfRangeException
    {
        using OutOfRangeException::OutOfRangeException;

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
            StackTrace stackTrace = StackTrace::current()) noexcept;

        Thrower& RecordStack(StackTrace stackTrace = StackTrace::current());

        Thrower& RecordSource(ExceptionSource src = ExceptionSource::Current());

        template <typename TEx>
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
