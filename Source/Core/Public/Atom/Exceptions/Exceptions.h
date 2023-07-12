#pragma once
#include "Exceptions.decl.h"
#include "Atom/Str/Str.h"
#include "Atom/Str/StrView.h"
#include "Atom/Str/AsciiStrView.h"

namespace Atom
{
    constexpr ExceptionSource ExceptionSource::Current(TImpl src) noex
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

    inline Exception::Exception(Str msg) noex:
        msg(MOVE(msg)) { }

    inline Exception::~Exception() noex { }

    inline IndexOutOfRangeException::IndexOutOfRangeException(
        Str msg, usize index, usize begin, usize end):
        OutOfRangeException(MOVE(msg)), index{ index }, begin{ begin }, end{ end } { }
}

namespace Atom::Ex::Internal
{
    inline Thrower::Thrower(ExceptionSource src, StackTrace stackTrace) noex:
        src{ MOVE(src) }, stackTrace{ MOVE(stackTrace) } { }

    inline Thrower& Thrower::RecordStack(StackTrace stackTrace)
    {
        this->stackTrace = MOVE(stackTrace);
        return *this;
    }

    inline Thrower& Thrower::RecordSource(ExceptionSource src)
    {
        this->src = MOVE(src);
        return *this;
    }

    template <tname TEx>
    requires RDerivedFrom<TEx, Exception>
    inline void Thrower::operator << (TEx&& ex)
    {
        ex.src = MOVE(src);
        ex.stackTrace = MOVE(stackTrace);

        throw ex;
    }
}
