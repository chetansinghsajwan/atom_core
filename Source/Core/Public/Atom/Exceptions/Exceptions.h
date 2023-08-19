#pragma once
#include "Atom/Core.h"

// #include <stacktrace>

namespace Atom
{
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
            return StackTrace{};
        }
    };

    /// --------------------------------------------------------------------------------------------
    /// 
    /// --------------------------------------------------------------------------------------------
    class ErrMsg
    {
    public:
        constexpr ErrMsg():
            msg{ "" } {}

        constexpr ErrMsg(const char* msg):
            msg{ msg } {}

        template <usize count>
        constexpr ErrMsg(const char(&msg)[count]):
            msg{ msg } {}

    public:
        const char* msg;
    };

    /// --------------------------------------------------------------------------------------------
    /// Base class for all errs.
    /// --------------------------------------------------------------------------------------------
    class Err
    {
    public:
        /// ----------------------------------------------------------------------------------------
        /// Basic constructor with msg.
        /// ----------------------------------------------------------------------------------------
        Err() = default;

        /// ----------------------------------------------------------------------------------------
        /// Basic constructor with msg.
        /// ----------------------------------------------------------------------------------------
        Err(ErrMsg msg):
            msg{ mov(msg) } {}

    public:
        /// ----------------------------------------------------------------------------------------
        /// Message explaining what went wrong.
        /// ----------------------------------------------------------------------------------------
        ErrMsg msg;
    };

    /// --------------------------------------------------------------------------------------------
    ///
    /// --------------------------------------------------------------------------------------------
    class RuntimeErr: public Err
    {
    public:
        using Err::Err;
    };

    /// --------------------------------------------------------------------------------------------
    /// Err represents invalid operation.
    /// --------------------------------------------------------------------------------------------
    class InvalidOperationErr: public Err
    {
    public:
        using Err::Err;
    };

    /// --------------------------------------------------------------------------------------------
    /// Err represents invalid argument.
    /// --------------------------------------------------------------------------------------------
    class InvalidArgErr: public Err
    {
    public:
        using Err::Err;
    };

    /// --------------------------------------------------------------------------------------------
    /// Err represents null pointer access.
    /// --------------------------------------------------------------------------------------------
    class NullPtrErr: public Err
    {
    public:
        using Err::Err;
    };

    /// --------------------------------------------------------------------------------------------
    /// Err represents an out of range access.
    /// --------------------------------------------------------------------------------------------
    class OutOfRangeErr: public Err
    {
    public:
        using Err::Err;
    };

    /// --------------------------------------------------------------------------------------------
    /// Err represents an out of range access, where range is representable using indices.
    /// --------------------------------------------------------------------------------------------
    class IndexOutOfRangeErr: public OutOfRangeErr
    {
    public:
        IndexOutOfRangeErr(ErrMsg msg, usize index, usize begin, usize end):
            OutOfRangeErr(msg), index{ index }, begin{ begin }, end{ end } {}

        IndexOutOfRangeErr(usize index, usize begin, usize end):
            OutOfRangeErr(), index{ index }, begin{ begin }, end{ end } {}

    public:
        usize index;
        usize begin;
        usize end;
    };
}
