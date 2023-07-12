#pragma once
#include "Exceptions.decl.h"
#include "Atom/Str/Str.h"
#include "Atom/Str/StrView.h"
#include "Atom/Str/AsciiStrView.h"

namespace Atom
{
    cexpr ExceptionSource ExceptionSource::Current(TImpl src) noex
    {
        return ExceptionSource
        {
            .line = src.line(),
            .column = src.column(),
            // TODO: Fix this usage.
            .fileName = Range(src.file_name(), usize(0)),
            .funcName = Range(src.function_name(), usize(0))
        };
    }

    inline Exception::ctor Exception(Str msg) noex:
        msg(MOVE(msg)) { }

    inline Exception::dtor Exception() noex { }

    inline IndexOutOfRangeException::ctor IndexOutOfRangeException(
        Str msg, usize index, usize begin, usize end):
        OutOfRangeException(MOVE(msg)), index{ index }, begin{ begin }, end{ end } { }
}

namespace Atom::Ex::Internal
{
    inline ctor Thrower::Thrower(ExceptionSource src, StackTrace stackTrace) noex:
        src{ MOVE(src) }, stackTrace{ MOVE(stackTrace) } { }

    inline fn Thrower::RecordStack(StackTrace stackTrace) -> Thrower&
    {
        this->stackTrace = MOVE(stackTrace);
        return *this;
    }

    inline fn Thrower::RecordSource(ExceptionSource src) -> Thrower&
    {
        this->src = MOVE(src);
        return *this;
    }

    template <tname TEx>
    requires RDerivedFrom<TEx, Exception>
    inline fn Thrower::operator << (TEx&& ex)
    {
        ex.src = MOVE(src);
        ex.stackTrace = MOVE(stackTrace);

        throw ex;
    }
}
